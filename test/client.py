import sys,zmq,time

sys.path.append("./python/inc")
 
context = zmq.Context()
socket = context.socket(zmq.REQ)  
socket.connect ("tcp://127.0.0.1:5555")  

while(True):
    socket.send_string("Hello")
    message = socket.recv()
    print("Received reply: %s" % message)
    time.sleep(1)
