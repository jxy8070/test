local sec = Timer.Now()  --1900-01-01开始的秒数，带3位小数点
print(sec)

--命名定时，以指定秒数周期执行EventHandler设定的函数
local t1= Timer.New()
t1.EventHandler = function()
--  Controls.Outputs[1].Boolean = not Controls.Outputs[1].Boolean
  print("t1.EventHandler")
end
t1:Start(2.5)

local t2= Timer.New()
t2.EventHandler = function()
--  Controls.Outputs[1].Boolean = not Controls.Outputs[1].Boolean
  print("t2.EventHandler")
end
t2:Start(5)

--简单定时，指定秒数后单次执行func1
local func1 = function()
  print("Timer.CallAfter")
end
Timer.CallAfter(func1, 2.0)