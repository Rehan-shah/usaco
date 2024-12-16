T = int(input())

def max(n:int):
  if n == 1:
    return 0
  return sum(int('5'*(r)) for r in range(1,n))

for _ in range(0,T): 
  n = input()
  u = len(n) 
  lowLi = int("4"*u) 
  uppLi = int("5"+"0"*(u-1))
  
  if int(n) <= lowLi: 
    print( max(u - 1))
  elif int(n) >= uppLi:
    print(max(u))
  else:
    print(max(u - 1)+ int(n) - lowLi)


  
    

    
