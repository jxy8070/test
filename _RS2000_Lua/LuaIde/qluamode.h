// -*- C++ -*-

#ifndef QLUAMODE_H
#define QLUAMODE_H

#include "qluatextedit.h"

#include <QList>
#include <QObject>
#include <QSize>
#include <QString>
#include <QStringList>
#include <QSyntaxHighlighter>
#include <QTextBlock>
#include <QTextBlockUserData>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextEdit>
#include <QVariant>
#include <QWidget>

#include "lua.hpp"

enum TokenTypeLua {
  // generic tokens
  Other, Identifier, Number, String,
  // tokens
  SemiColon, ThreeDots, Comma, Dot, Colon,
  LParen, RParen, LBracket, RBracket, LBrace, RBrace,
  // keywords
  Kand, Kfalse, Kfunction, Knil,
  Knot, Kor, Ktrue, Kin,
  // keywords that kill statements
  Kbreak, Kdo, Kelse, Kelseif, Kend, Kfor,
  Kif, Klocal, Krepeat, Kreturn,
  Kthen, Kuntil, Kwhile,
  // special
  Eof,
  Chunk,
  Statement,
  StatementCont,
  FunctionBody,
  FunctionName,
  FirstKeyword = Kand,
  FirstStrictKeyword = Kbreak,
};

struct Keywords {
  const char *text;
  TokenTypeLua type;
};


struct Node;

struct PNode : public QSharedDataPointer<Node> {
  PNode();
  PNode(TokenTypeLua t, int p, int l, PNode n);
  PNode(TokenTypeLua t, int p, int l, int i, PNode n);
  TokenTypeLua type() const;
  int pos() const;
  int len() const;
  int indent() const;
  PNode next() const;
};

struct Node : public QSharedData {
  Node(TokenTypeLua t, int p, int l, PNode n)
    : next(n), type(t),pos(p),len(l),indent(-1) {}
  Node(TokenTypeLua t, int p, int l, int i, PNode n)
    : next(n), type(t),pos(p),len(l),indent(i) {}
  PNode next;
  TokenTypeLua type;
  int pos;
  int len;
  int indent;
};

class QTIDE_API QLuaModeUserData : public QTextBlockUserData
{
public:
  virtual int highlightState();
};


// Class to store user data for each block
struct UserData : public QLuaModeUserData
{
  // lexical state
  int lexState;
  int lexPos;
  int lexN;
  // parser state
  PNode nodes;
  int lastPos;
  // initialize
  UserData() : lexState(0), lexPos(0), lexN(0), lastPos(0) {}
  virtual int highlightState() { return (lexState<<16)^lexN; }
};



// A convenient base class for creating modes
class QTIDE_API QLuaMode : public QLuaTextEditMode
{
  Q_OBJECT
public:
  QLuaMode(QLuaTextEditModeFactory *f, QLuaTextEdit *e);
  virtual QSyntaxHighlighter *highlighter();
  virtual bool supportsHighlight() { return true; }
  virtual bool supportsMatch() { return true; }
  virtual bool supportsBalance() { return true; }
  virtual bool supportsIndent() { return true; }
protected:
  virtual void setFormat(int pos, int len, QString format);
  virtual void setFormat(int pos, int len, QTextCharFormat format);
  virtual void setLeftMatch(int pos, int len);
  virtual void setMiddleMatch(int pos, int len, int pp, int pl);
  virtual void setRightMatch(int pos, int len, int pp, int pl);
  virtual void setErrorMatch(int pos, int len, int pp, int pl, bool h=true);
  virtual  int followMatch(int pos, int len);
  virtual void setBalance(int fpos, int tpos);
  virtual void setBalance(int fpos, int tpos, bool outer);
  virtual void setIndent(int pos, int indent);
  virtual void setIndentOverlay(int pos, int indent=-2);
  virtual  int askCompletion(QString stem, QStringList comps);
  virtual void fileCompletion(QString &stem, QStringList &comps);
protected:
  virtual void parseBlock(int pos, const QTextBlock &block, 
                          const QLuaModeUserData *idata, 
                          QLuaModeUserData *&odata ) = 0;
public slots:
  virtual bool doTab();
  virtual bool doEnter();
  virtual bool doMatch();
  virtual bool doBalance();
public:
  class CompModel;
  class CompView;
  class Highlighter;
  class Private;
 private:
  Private *d;
};


// A template class for creating mode factories


template<class T>
class QTIDE_API QLuaModeFactory : public QLuaTextEditModeFactory
{
public:
  QLuaModeFactory(const char *n, const char *s) 
    : QLuaTextEditModeFactory(n,s) {}

  virtual QLuaTextEditMode *create(QLuaTextEdit *parent) 
    { return new T(this,parent); }
};


#endif




/* -------------------------------------------------------------
   Local Variables:
   c++-font-lock-extra-types: ("\\sw+_t" "\\(lua_\\)?[A-Z]\\sw*[a-z]\\sw*")
   End:
   ------------------------------------------------------------- */

