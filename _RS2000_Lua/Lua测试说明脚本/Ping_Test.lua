pingObj = Ping.New("192.168.1.204")
pingObj:start(false)
--pingObj:start(true)
pingObj:setTimeoutInterval(2.0)
pingObj:setPingInterval(5.0)
pingObj.EventHandler = function(response)
  print(response.HostName)
  print(response.ElapsedTime)
end
pingObj.ErrorHandler = function(response)
  print(response.HostName)
  print(response.Error)
end
