class Tester:
    def __init__(self, id):
        self.n = 0
        self.id = id
        print(self.id)
        print(self.n)
    def incre2(self, num):
        print('count to {}'.format(num))
        while self.n < num:
            if self.n%100000 == 0:
                print ('n of thread {} at this time self.n is {}'.format(self.id, self.n))
            self.n = self.n + 1      
        print('now self.n of thread{} is {}'.format(self.id, self.n))
            

cdef public createInstance(id):
    print('create')
    return Tester(id)

cdef public testprocess(object p):
    print('run cdef')
    p.incre2(100000000)

