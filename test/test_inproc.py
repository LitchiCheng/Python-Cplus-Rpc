import sys,zmq,time,json,threading

ctx = zmq.Context()

def runThread1():
    # ctx = zmq.Context()
    socket = ctx.socket(zmq.REP)
    socket.bind("inproc://#11")
    while True:
        print("thread1 rec " + socket.recv_string())
        socket.send_string("send from thread1")
        # time.sleep(1)

def runThread2():
    # ctx = zmq.Context()
    socket = ctx.socket(zmq.REQ)
    socket.connect("inproc://#11")
    while True:
        socket.send_string("send from thread2")
        print("thread2 rec " + socket.recv_string())

if __name__ == '__main__':
    thread1 = threading.Thread(None, runThread1, "thread1")
    thread2 = threading.Thread(None, runThread2, "thread2")
    thread1.start()
    thread2.start()