'''
Date: 2023-02-28 23:36:23
LastEditTime: 2023-03-01 07:22:42
FilePath: \Python-Cplus-Rpc\python\src\rpc_server.py
Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
'''
import zmq,json,threading,sys,time

class zmqServer(object):
    def __init__(self):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REP)
        self.socket.bind("ipc:///tmp/test1.ipc")
        self.data = None
        self.__should_close = False
        self.msg_thread = threading.Thread(target=self.__loop, name="loop")
        self.msg_thread.start()
    
    def close(self):
        print("close the socket")
        self.socket.close()
    
    def send(self, data):
        self.socket.send(data)

    def recv(self):
        data = self.socket.recv()
        return data

    def __loop(self):
        while True:
            try:
                print("rrrrr1")
                message = self.socket.recv()
                print("rrrrr2")
                self.data = json.loads(message.decode('utf-8'))
                method_name = self.data['method_name']
                res = self.funs[method_name]()
                data = {"res": res}
                self.socket.send(json.dumps(data).encode('utf-8'))
                if self.__should_close:
                    break
            except Exception as e:
                print('loop error',e)
    
    def shoutDown(self):
        self.__should_close = True

class rpcStub(object):
    def __init__(self):
        self.funs = {}

    def registerFunction(self, function, name=None):
        if name is None:
            name = function.__name__
        self.funs[name] = function

class rpcServer(zmqServer, rpcStub):
    def __init__(self):
        zmqServer.__init__(self)
        rpcStub.__init__(self)

if __name__ == '__main__':
    class Test:
        def setOn(self):
            print("is me ")
        
        def exit(self):
            print("exit")
            sys.exit()

    a = Test()
    test = rpcServer()
    test.registerFunction(a.setOn, "setOn")
    test.registerFunction(a.exit, "exit")
    while 1:
        print("alive")
        time.sleep(1)
    test.shoutDown()


