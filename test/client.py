import sys,zmq,time,json

sys.path.append("./python/inc")

class zmqClient(object):
    def __init__(self):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.addr = "tcp://127.0.0.1:5555"
    
    def connect(self, addr):
        self.addr = addr
        self.socket.connect(addr)
    
    def send(self, data):
        self.socket.send(data)

    def recv(self):
        data = self.socket.recv()
        return data

class rpcStub(object):
    def __getattr__(self, function):
        def _func(*args, **kwargs):
            d = {'method_name': function, 'method_args': args, 'method_kwargs': kwargs}
            self.send(json.dumps(d).encode('utf-8'))
            data = self.recv()
            return data

        setattr(self, function, _func)
        return _func

class rpcClient(zmqClient, rpcStub):
    pass

test = rpcClient()
test.connect("ipc:///tmp/test0.ipc")

test2 = zmqClient()
xx = 0

while(True):
    xx += 1
    res = test.add(xx, 2, c=3)
    reply = json.loads(res.decode())
    print(str(time.asctime(time.localtime(time.time()))))
    print(reply["res"])
    time.sleep(0.001)


