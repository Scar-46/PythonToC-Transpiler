def suma(a,b): 
    return ((a)+(b)) 
 
def factorial(n): 
    if n < 1: 
        return 1 
    else: 
        return n * factorial(n-1) 
 
a = dict() 
a["hola"] = "mundo" 

b = { 
    "hello": "world", 
    "spanish": a, 
    "empty": dict(), 
    "set": set(), 
    1: "1", 
    "list": list(), 
} 
 
def while_func(n): 
    p = n + 1
    while (p > 0): 
        print(p) 
        p=p-1 
        if p == 10: 
            break 
        if p == 7: 
            continue 
        if p >= 50000: 
            return "Hola" 
        elif p > 1000: 
            return 1 

def ifs(a,b,c,d): 
    if a or b: 
        print(a) 
    elif c: 
        if a and b: 
            if d: 
                print(d) 
        elif a: 
            if d: 
                print(c   ) 
        else: 
            print (   a) 

hi = hola = "hola" + " mundo" + "!" 
print(hi) 
print(hola) 
 
def hello(): 
    print("hello") 
    mundo() 
 
def mundo(): 
    print("mundo")