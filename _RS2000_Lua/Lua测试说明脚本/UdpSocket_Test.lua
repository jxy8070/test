udp = UdpSocket.New()  --Create new UDP object
 
udp.EventHandler = function(udp, packet)
    print( packet.Address, packet.Port, packet.Data )
end
 
udp:Open( "192.168.1.100", 8888) 
udp:Send( "192.168.1.100", 9999, "test send") 