import matplotlib.pyplot as plt
from scipy import interpolate
import numpy as np


import matplotlib.font_manager as mpt

zhfont=mpt.FontProperties(fname='/usr/share/fonts/custom/msyh.ttf') 

file = 'record-3t.txt'
a = np.loadtxt(file)
x = a[:,0]  
y = a[:,1]  
tck = interpolate.splrep(x,y)
xx = np.linspace(min(x),max(x),100)
yy = interpolate.splev(xx,tck,der=0)
print(xx)
plt.plot(x,y,'o',xx,yy)
plt.legend(['true','Cubic-Spline'])
plt.xlabel('distance(cm)', fontproperties=zhfont) 
plt.ylabel('%')
plt.title('Dram', fontproperties=zhfont)  
plt.savefig('out.jpg')
plt.show()
