/* -*- C++ -*- */

#include <QtGlobal>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QActionGroup>
#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontMetrics>
#include <QFont>
#include <QFontInfo>
#include <QKeyEvent>
#include <QLineEdit>
#include <QList>
#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPaintEvent>
#include <QPointer>
#include <QPushButton>
#include <QRegExp>
#include <QSettings>
#include <QShortcut>
#include <QString>
#include <QStringList>
#include <QSyntaxHighlighter>
#include <QTemporaryFile>
#include <QTextBlock>
#include <QTextBlockUserData>
#include <QTextEdit>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextFrameFormat>
#include <QTextLayout>
#include <QTextOption>
#include <QTimer>
#include <QToolBar>
#include <QVariant>
#include <QVBoxLayout>
#include <QWhatsThis>


#include "qluatextedit.h"




// ========================================
// QLUATEXTEDITMODE



QLuaTextEditMode::QLuaTextEditMode(QLuaTextEditModeFactory *f, QLuaTextEdit *e)
  : e(e), f(f)
{
}


QLuaTextEditModeFactory *QLuaTextEditModeFactory::first = 0;
QLuaTextEditModeFactory *QLuaTextEditModeFactory::last = 0;


QLuaTextEditModeFactory::~QLuaTextEditModeFactory()
{
  if (prev) { prev->next = next; } else { first = next; }
  if (next) { next->prev = prev; } else { last = prev; }
}


QLuaTextEditModeFactory::QLuaTextEditModeFactory(const char *name, 
                                                 const char *suffixes)
  : name_(name), suffixes_(suffixes), next(first), prev(0)
{
  // find sorted position
  while (next && strcmp(next->name_, name_) < 0)
    { prev = next; next = next->next; }
  // insert
  if (prev) { prev->next = this; } else { first = this; }
  if (next) { next->prev = this; } else { last = this; }
}


QString 
QLuaTextEditModeFactory::name()
{
  return QLuaTextEditMode::tr(name_);
}


QString 
QLuaTextEditModeFactory::filter()
{
  QString patterns = "*." + suffixes().join(" *.");
  return QLuaTextEditMode::tr("%1 Files (%2)").arg(name()).arg(patterns);
}


QStringList 
QLuaTextEditModeFactory::suffixes()
{
  return QString(suffixes_).split(';');
}


QList<QLuaTextEditModeFactory*> 
QLuaTextEditModeFactory::factories()
{
  QList<QLuaTextEditModeFactory*> factories;
  for (QLuaTextEditModeFactory *f = first; f; f=f->next)
    factories += f;
  return factories;
}



// ========================================
// LINENUMBERS


class QLuaTextEdit::LineNumbers : public QWidget
{
  Q_OBJECT
  QLuaTextEdit *e;
public:
  LineNumbers(QLuaTextEdit *e, QWidget *p);
  virtual QSize sizeHint() const;
public slots:
  void updateRequest(const QRect &rect, int dy);

protected:
  void paintEvent(QPaintEvent *event) override;
};


QLuaTextEdit::LineNumbers::LineNumbers(QLuaTextEdit *e, QWidget *p)
  : QWidget(p), e(e)
{
  connect(e, SIGNAL(updateRequest(const QRect&, int)),
          this, SLOT(updateRequest(const QRect&, int)) );

  setAttribute(Qt::WA_StyledBackground,true);
}

QSize
QLuaTextEdit::LineNumbers::sizeHint() const
{
  QString s = "00000";
  QFont font = e->font();
  font.setWeight(QFont::Bold);
  QFontMetrics metrics(font);
  return QSize(metrics.width(s), 0);
}


void
QLuaTextEdit::LineNumbers::updateRequest(const QRect &rect, int dy)
{
  if (isHidden())
    return;
  if (dy != 0)
    scroll(0, dy);
  else
    {
      QRect r = rect;
      r.setLeft(0);
      r.setWidth(width());
      QPointF offset = e->contentOffset();
      for (QTextBlock block = e->firstVisibleBlock(); 
           block.isValid(); block=block.next() )
        {
          if (! block.isVisible())
            continue;
          QRectF rf = e->blockBoundingGeometry(block).translated(offset);
          if (rf.bottom() < rect.top())
            continue;
          if (rf.top() > rect.bottom())
            break;
          r.setTop(qMin(r.top(), (int)rf.top()));
          r.setBottom(qMax(r.bottom(), (int)rf.bottom()));
        }
      update(r);
    }
}


void
QLuaTextEdit::LineNumbers::paintEvent(QPaintEvent *event)
{
  QRect er = event->rect();
  QPointF offset = e->contentOffset();
  QPainter painter(this);
  QFont font = e->font();
//  font.setWeight(QFont::Bold);
  painter.setFont(font);
  painter.setPen(QColor(196,206,189));
  for (QTextBlock block = e->firstVisibleBlock(); 
       block.isValid(); block=block.next() )
    {
      int n = block.blockNumber();
      if (! block.isVisible())
        continue;
      QRectF r = e->blockBoundingGeometry(block).translated(offset);
      if (r.bottom() < er.top())
        continue;
      if (r.top() > er.bottom())
        break;
      r.setLeft(0);
      r.setWidth(width());
      QString s = QString("%1 ").arg(n+1);
      painter.drawText(r, Qt::AlignRight|Qt::AlignTop|Qt::TextSingleLine, s);
    }
}



// ========================================
// QLUATEXTEDIT


class QLuaTextEdit::Private : public QObject
{
  Q_OBJECT
public:
  ~Private();
  Private(QLuaTextEdit *q);
  QString filterText(QString);
  void expandTab();
  bool eventFilter(QObject *watched, QEvent *event);
  static QTextCharFormat defaultFormat(QString);
  static void saveFormats();
public slots:
  void updateMatch();
  void updateHighlight();
  void scheduleLayout();
  void layout();
public:
  QLuaTextEdit *q;
  LineNumbers *lineNumbers;
  bool showLineNumbers;
  bool autoComplete;
  bool autoIndent;
  bool autoHighlight;
  bool autoMatch;
  bool tabExpand;
  int tabSize;
  QSize sizeInChars;
  bool layoutScheduled;
  QPointer<QLuaTextEditMode> mode;
  QPointer<QSyntaxHighlighter> highlighter;
};


QLuaTextEdit::Private::~Private()
{
  saveFormats();
  delete highlighter;
  delete mode;
}


QLuaTextEdit::Private::Private(QLuaTextEdit *q)
  : QObject(q), 
    q(q),
    lineNumbers(0),
    showLineNumbers(true),
    autoComplete(true), 
    autoIndent(true), 
    autoHighlight(true), 
    autoMatch(true),
    tabExpand(false), 
    tabSize(4),
    sizeInChars(QSize(80,25)),
    layoutScheduled(false)
{
  connect(q, SIGNAL(cursorPositionChanged()),
          this, SLOT(updateMatch()));
}


QString
QLuaTextEdit::Private::filterText(QString data)
{
  int pos = 0;
  int lastic = 0;
  QString dest;
  dest.reserve(data.size());
  for (int i=0; i<data.size(); i++)
    {
      QChar c = data.at(i);
      int ic = c.toLatin1();
      if (ic == '\t')
        {
          int tpos = (int)((pos + tabSize) / tabSize) * tabSize;
          if (! tabExpand)
            dest += c;
          while (pos++ < tpos)
            if (tabExpand)
              dest += ' ';
        }
      else if (ic == '\n' || ic == '\r')
        {
          if (ic != '\n' || lastic != '\r')
            dest += '\n';
          pos = 0;
        }
      else if (isprint(ic) || isspace(ic) || c.isPrint() || c.isSpace())
        {
          dest += c;
          pos += 1;
        }
      lastic = ic;
    }
  dest.squeeze();
  return dest;
}


void
QLuaTextEdit::Private::expandTab()
{
  QTextCursor cursor = q->textCursor();
  cursor.beginEditBlock();
  if (cursor.hasSelection())
    cursor.deleteChar();
  int pos = cursor.position() - cursor.block().position();
  cursor.insertText(QString(tabSize - (pos % tabSize), QChar(' ')));
  cursor.endEditBlock();
}


void 
QLuaTextEdit::Private::updateMatch()
{
  bool b = false;;
  if (mode && mode->supportsMatch() && autoMatch)
    b = mode->doMatch();
  QList<QTextEdit::ExtraSelection> empty;
  if (!b && !q->extraSelections().isEmpty())
    q->setExtraSelections(empty);
}


void 
QLuaTextEdit::Private::updateHighlight()
{
  delete highlighter;
  if (mode && mode->supportsHighlight() && autoHighlight)
    highlighter = mode->highlighter();
  if (highlighter)
    QTimer::singleShot(0, highlighter, SLOT(rehighlight()));
}


bool 
QLuaTextEdit::Private::eventFilter(QObject *watched, QEvent *event)
{
  if (event->type() == QEvent::Resize 
      && watched == q->viewport() )
    scheduleLayout();
  return false;
}


void
QLuaTextEdit::Private::scheduleLayout()
{
  if (! layoutScheduled)
    QTimer::singleShot(0, this, SLOT(layout()));
  layoutScheduled = true;
}


void
QLuaTextEdit::Private::layout()
{
  layoutScheduled = false;
  if (! showLineNumbers)
    {
      if (lineNumbers)
        lineNumbers->hide();
      q->setViewportMargins(0, 0, 0, 0);
    }
  else
    {
      if (! lineNumbers)
        lineNumbers = new LineNumbers(q, q);
      int w = lineNumbers->sizeHint().width();
      q->setViewportMargins(w, 0, 0, 0);
      QRect r = q->viewport()->rect();
      r.setLeft(0);
      r.setWidth(w);
      lineNumbers->setGeometry(r);
      lineNumbers->show();
    }
}


QLuaTextEdit::QLuaTextEdit(QWidget *parent)
  : QPlainTextEdit(parent), d(new Private(this))
{
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  setLineWrapMode(NoWrap);
  viewport()->installEventFilter(d);
  connect(this, SIGNAL(blockCountChanged(int)), d, SLOT(scheduleLayout()));
  d->layout();
}


bool 
QLuaTextEdit::showLineNumbers() const
{
  return d->showLineNumbers;
}


bool 
QLuaTextEdit::autoComplete() const
{
  return d->autoComplete;
}


bool 
QLuaTextEdit::autoIndent() const
{
  return d->autoIndent;
}


bool 
QLuaTextEdit::autoHighlight() const
{
  return d->autoHighlight;
}


bool 
QLuaTextEdit::autoMatch() const
{
  return d->autoMatch;
}


bool 
QLuaTextEdit::tabExpand() const
{
  return d->tabExpand;
}


int 
QLuaTextEdit::tabSize() const
{
  return d->tabSize;
}


QSize 
QLuaTextEdit::sizeInChars() const
{
  return d->sizeInChars;
}


QLuaTextEditMode *
QLuaTextEdit::editorMode() const
{
  return d->mode;
}


void
QLuaTextEdit::setShowLineNumbers(bool b)
{
  if (d->showLineNumbers == b)
    return;
  d->showLineNumbers = b;
  d->scheduleLayout();
  emit settingsChanged();
}


void
QLuaTextEdit::setAutoComplete(bool b)
{
  if (d->autoComplete == b)
    return;
  d->autoComplete = b;
  emit settingsChanged();
}


void
QLuaTextEdit::setAutoIndent(bool b)
{
  if (d->autoIndent == b)
    return;
  d->autoIndent = b;
  emit settingsChanged();
}


void
QLuaTextEdit::setAutoHighlight(bool b)
{
  if (d->autoHighlight == b)
    return;
  d->autoHighlight = b;
  d->updateHighlight();
  emit settingsChanged();
}


void
QLuaTextEdit::setAutoMatch(bool b)
{
  if (d->autoMatch == b)
    return;
  d->autoMatch = b;
  d->updateMatch();
  emit settingsChanged();
}


void 
QLuaTextEdit::setTabExpand(bool b)
{
  if (d->tabExpand == b)
    return;
  d->tabExpand = b;
  emit settingsChanged();
}


void 
QLuaTextEdit::setTabSize(int s)
{
  if (s > 1 && s < 48 && s != d->tabSize)
    {
      d->tabSize = s;
      updateGeometry();
      emit settingsChanged();
    }
}


void 
QLuaTextEdit::setSizeInChars(QSize size)
{
  d->sizeInChars = size;
  updateGeometry();
}


bool
QLuaTextEdit::setEditorMode(QLuaTextEditModeFactory *factory)
{
  delete d->highlighter;
  delete d->mode;
  if (factory)
    d->mode = factory->create(this);
  d->updateMatch();
  d->updateHighlight();
  emit settingsChanged();
  return true;
}


bool 
QLuaTextEdit::setEditorMode(QString suffix)
{
  QLuaTextEditModeFactory *factory = 0;
  QList<QLuaTextEditModeFactory*> list = QLuaTextEditModeFactory::factories();
  foreach(QLuaTextEditModeFactory *f, list)
    if (f->suffixes().contains(suffix))
      factory = f;
  suffix = suffix.toLower();
  if (! factory)
    foreach(QLuaTextEditModeFactory *f, list)
      if (f->suffixes().contains(suffix))
        factory = f;
  if (factory)
    return setEditorMode(factory);
  return false;
}


bool 
QLuaTextEdit::readFile(QFile &file)
{
  if (file.open(QIODevice::ReadOnly))
    {
      QTextStream in(&file);
      QApplication::setOverrideCursor(Qt::WaitCursor);
      QString data = d->filterText(in.readAll());
      if (! file.error())
        {
          setPlainText(data);
          QApplication::restoreOverrideCursor();
          return true;
        }
      QApplication::restoreOverrideCursor();
    }
  return false;
}


bool 
QLuaTextEdit::readFile(QString fname)
{
  QFile file(fname);
  return readFile(file);
}


bool 
QLuaTextEdit::writeFile(QFile &file)
{
  if (file.open(QIODevice::WriteOnly))
    {
      QTextStream out(&file);
      QApplication::setOverrideCursor(Qt::WaitCursor);
      out << toPlainText();
      QApplication::restoreOverrideCursor();
      if (! file.error())
        return true;
    }
  return false;
}


bool 
QLuaTextEdit::writeFile(QString fname)
{
  QFile file(fname);
  return writeFile(file);
}


void 
QLuaTextEdit::showLine(int lineno)
{
  QTextDocument *d = document();
  QTextBlock block = d->findBlockByNumber(lineno-1);
  if (block.isValid())
    {
      window()->raise();
      window()->activateWindow();
      QTextCursor c(d);
      c.setPosition(getBlockIndent(block));
      setTextCursor(c);
      centerCursor();
      QList<QTextEdit::ExtraSelection> lextra;
      QTextEdit::ExtraSelection extra;
      c.setPosition(block.position()+block.length()-1, QTextCursor::KeepAnchor);
      extra.cursor = c;
      extra.format = format("(showline)/error");
      setExtraSelections(lextra << extra);
      return;
    }
  QApplication::beep();
}


QSize 
QLuaTextEdit::sizeHint() const
{
  QFontMetrics fontMetrics(font());
  int cellw = fontMetrics.width("MM") - fontMetrics.width("M");
  int cellh = fontMetrics.lineSpacing();
  int w = d->sizeInChars.width();
  int h = d->sizeInChars.height();
  w += 4;
  if (d->showLineNumbers)
    w += 5;
  int t = cellw * d->tabSize;
  if (t != tabStopWidth())
    const_cast<QLuaTextEdit*>(this)->setTabStopWidth(t);
  return QSize(cellw * w, cellh * h);
}


void
QLuaTextEdit::keyPressEvent(QKeyEvent *event)
{
  // overwrite mode
  if (event->key() == Qt::Key_Insert &&
      event->modifiers() == Qt::NoModifier)
    {
      setOverwriteMode(! overwriteMode());
      emit settingsChanged();
      return;
    }
  // autoindent and autocomplete
  QString s = event->text();
  if (s == "\n" || s == "\r")
    {
      if (d->autoIndent && d->mode && d->mode->supportsIndent())
        d->mode->doEnter();
      else
        QPlainTextEdit::keyPressEvent(event);
      return;
    }
  else if (s == "\t")
    {
      bool ok = false;
      if (d->autoIndent)
        if (d->mode && d->mode->supportsIndent())
          ok = d->mode->doTab() || ok;
      if (d->autoComplete)
        if (d->mode && d->mode->supportsComplete())
          if (! textCursor().hasSelection())
            ok = d->mode->doComplete() || ok;
      if (! ok)
        QPlainTextEdit::keyPressEvent(event);
      return;
    }
  // default
  QPlainTextEdit::keyPressEvent(event);
}


int 
QLuaTextEdit::indentAt(int pos)
{
  QTextBlock block = document()->findBlock(pos);
  return indentAt(pos, block);
}


int 
QLuaTextEdit::indentAt(int pos, QTextBlock block)
{
  int c = 0;
  int bpos = block.position();
  if (block.isValid() && pos>=bpos)
    {
      int ts = d->tabSize;
      QString text = block.text();
      int ss = text.size();
      int e = pos-block.position();
      for (int i=0; i<e && i<ss; i++)
        if (text[i].toLatin1() == '\t')
          c = ((int)(c / ts) + 1) * ts;
        else
          c = c + 1;
    }
  return c;
}


int 
QLuaTextEdit::indentAfter(int pos, int dpos)
{
  QTextBlock block = document()->findBlock(pos);
  if (block.isValid())
    {
      QString text = block.text();
      int i = pos - block.position();
      for (; i< text.size(); i++)
        if (! text[i].isSpace())
          break;
      if (i < text.size())
        return indentAt(block.position() + i);
    }
  return indentAt(pos) + dpos;
}


int
QLuaTextEdit::getBlockIndent(QTextBlock block)
{
  int indent;
  return getBlockIndent(block, indent);
}


int
QLuaTextEdit::getBlockIndent(QTextBlock block, int &indent)
{
  
  Q_ASSERT(block.isValid());
  indent = -1;
  QString text = block.text();
  int ss = text.size();
  int ts = d->tabSize;
  int i;
  indent = 0;
  for (i=0; i<ss && text[i].isSpace(); i++)
    indent += (text[i].toLatin1() == '\t') ? ts : 1;
  if (i >= ss)
    indent = -1;
  return block.position() + i;
}


int
QLuaTextEdit::setBlockIndent(QTextBlock block, int indent)
{
  Q_ASSERT(block.isValid());
  int oindent;
  int cpos = getBlockIndent(block, oindent);
  if (oindent == indent)
    return cpos;
  QTextCursor cursor(block);
  cursor.setPosition(cpos, QTextCursor::KeepAnchor);
  QString spaces;
  if (!d->tabExpand && d->tabSize > 0)
    for (; indent >= d->tabSize; indent -= d->tabSize)
      spaces += '\t';
  for(; indent>=1; indent-=1)
    spaces += ' ';
  cursor.insertText(spaces);
  return cursor.position();
}



QRectF 
QLuaTextEdit::blockBoundingGeometry(const QTextBlock &block) const
{
  // just to make this function public!
  return QPlainTextEdit::blockBoundingGeometry(block);
}




// ========================================
// FORMATS


typedef QMap<QString,QTextCharFormat> Formats;


Q_GLOBAL_STATIC(Formats, formats);

//highlight color
QTextCharFormat 
QLuaTextEdit::Private::defaultFormat(QString key)
{
  QTextCharFormat fmt;
  if (key.endsWith("/comment")) {
    fmt.setFontItalic(true);
    fmt.setForeground(QColor(98,246,9));
  } else if (key.endsWith("/string")) {
    fmt.setForeground(Qt::red);
  } else if (key.endsWith("/keyword")) {
    fmt.setForeground(QColor(64,196,255));
    fmt.setFontWeight(QFont::Bold);
  } else if (key.endsWith("/cpp")) {
    fmt.setForeground(Qt::magenta);
  } else if (key.endsWith("/function")) {
    fmt.setForeground(Qt::darkCyan);
    fmt.setFontWeight(QFont::Bold);
  } else if (key.endsWith("/quote")) {
    fmt.setForeground(Qt::red);
  } else if (key.endsWith("/type")) {
    fmt.setForeground(Qt::red);
  } else if (key.endsWith("/url")) {
    fmt.setForeground(Qt::red);
  } else if (key.endsWith("/error")) {
    fmt.setBackground(QColor(255,127,127));
  } else if (key.startsWith("(matcher)/")) {
    fmt.setBackground(QColor(240,240,64));
  }
  return fmt;
}


QTextCharFormat 
QLuaTextEdit::format(QString key)
{
  // quick cache
  if (formats()->contains(key))
    return (*formats())[key];
  // defaults
  QTextCharFormat fmt = Private::defaultFormat(key);
  // settings
  QSettings s;
  s.beginGroup("formats");
  s.beginGroup(key);
  if (s.contains("italic"))
    fmt.setFontItalic(s.value("italic").toBool());
  if (s.contains("weight")) {
    int i = s.value("weight").toInt();
    if ((i >= QFont::Light) && (i <= QFont::Black))
      fmt.setFontWeight(i);
  }
  if (s.contains("color")) {
    QColor c;
    c.setNamedColor(s.value("color").toString());
    if (c.isValid())
      fmt.setForeground(c);
  }
  if (s.contains("bgcolor")) {
    QColor c;
    c.setNamedColor(s.value("bgcolor").toString());
    if (c.isValid())
      fmt.setBackground(c);
  }
  // cache and return
  (*formats())[key] = fmt;
  return fmt;
}

void
QLuaTextEdit::setFormat(QString key, QTextCharFormat fmt)
{
  (*formats())[key] = fmt;
}

void 
QLuaTextEdit::Private::saveFormats()
{
  QSettings s;
  s.beginGroup("formats");
  Formats::const_iterator it = formats()->begin();
  for(; it != formats()->end(); ++it)
    {
      QString key = it.key();
      QTextCharFormat fmt = it.value();
      QTextCharFormat dfmt = defaultFormat(key);
      if (fmt != dfmt)
        {
          s.beginGroup(key);
          s.setValue("italic", fmt.fontItalic());
          s.setValue("weight", fmt.fontWeight());
          s.setValue("color", fmt.foreground().color());
          s.setValue("bgcolor", fmt.background().color());
          s.endGroup();
        }
    }
}


void 
QLuaTextEdit::reHighlight()
{
  formats()->clear();
  d->updateHighlight();
}



// ========================================
// MOC


#include "qluatextedit.moc"





/* -------------------------------------------------------------
   Local Variables:
   c++-font-lock-extra-types: ("\\sw+_t" "\\(lua_\\)?[A-Z]\\sw*[a-z]\\sw*")
   End:
   ------------------------------------------------------------- */
