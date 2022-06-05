import sys
from PyQt5 import QtCore, QtWidgets
from PyQt5.QtWidgets import QWidget,QPushButton,QApplication,QListWidget,QGridLayout,QLabel,QLineEdit
from PyQt5.QtCore import QTimer
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg
from matplotlib.figure import Figure
import matplotlib.pyplot as plt

class MplCanvas(FigureCanvasQTAgg):
    def __init__(self, parent=None, width=5, height=4, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(131)
        self.axes1 = fig.add_subplot(132)
        self.axes2 = fig.add_subplot(133)
        super(MplCanvas, self).__init__(fig)

class WinForm(QWidget):        
    def __init__(self,parent=None):
        super(WinForm, self).__init__(parent)
        self.setWindowTitle('one-dimensional chain')
        self.createSetupWindow()
        self.c = 0
        self.a = 0
        self.m = 0
        self.length = 0.1
        self.elementAmount = 20
        self.x_step = self.length/(self.elementAmount-1)
        self.x = self.getStartingXs()
        self.y = []
        for i in range(self.elementAmount):
            self.y.append(self.getStartingYs(self.x[i]))
        self.dt = 0.01
        self.v_x = self.elementAmount*[0]
        self.v_y = self.elementAmount*[0]
        self.current_time = 0
        self.fps = 24
        self.count = 1
        self.speed_mod = 1/6
        print(round(self.x_step,5))

    def getStartingXs(self):
        a = []
        for i in range(self.elementAmount):
            a.append(i*self.x_step)
        #a[5]+=self.x_step/2
        return a

    def getStartingYs(self,x):
        #return pow(x,2)
        #return -pow((x-0.05),2)
        #return 0
        if(x<self.length/2):
            return (x*0.02)
        if(x>=self.length/2):
            return (0.02*self.length - (x-0*self.length)*0.02)
                
    def createSetupWindow(self):
        self.label=QLabel('Enter your system parameters')
        self.label1=QLabel('tension c, N/m')
        self.label2=QLabel('distance a, m')
        self.label3=QLabel('mass m, kg')
        self.input_1=QLineEdit()
        self.input_2=QLineEdit()
        self.input_3=QLineEdit()
        self.nextBtn=QPushButton('Next')

        self.layout=QGridLayout()

        self.layout.addWidget(self.label,0,0,1,3)
        self.layout.addWidget(self.input_1,1,0)
        self.layout.addWidget(self.input_2,1,1)
        self.layout.addWidget(self.input_3,1,2)
        self.layout.addWidget(self.label3,2,0)
        self.layout.addWidget(self.label2,2,1)
        self.layout.addWidget(self.label1,2,2)
        self.layout.addWidget(self.nextBtn,3,0)

        self.nextBtn.clicked.connect(self.createModelWindow)

        self.setLayout(self.layout)

        self.setGeometry(200, 200, 1520, 680)

    def updateCoords(self):
        for i in range(1,self.elementAmount-1):
            G_1 = pow((pow((self.y[i]-self.y[i-1]),2)+pow(self.a+self.x[i]-self.x[i-1],2)),0.5) #Г_n-1
            G_2 = pow((pow((self.y[i+1]-self.y[i]),2)+pow(self.a+self.x[i+1]-self.x[i],2)),0.5) #Г_n+1
            sin_1 = (self.y[i+1]-self.y[i])/G_2
            sin_2 = (self.y[i]-self.y[i-1])/G_1
            cos_1 = pow(1-sin_1*sin_1,0.5)
            cos_2 = pow(1-sin_2*sin_2,0.5)
            self.v_x[i] += (self.c/self.m*(self.x[i+1]-2*self.x[i]+self.x[i-1])+self.c*self.a/self.m*(cos_2-cos_1))*self.dt
            self.v_y[i] += (self.c/self.m*(self.y[i+1]-2*self.y[i]+self.y[i-1])+self.c*self.a/self.m*(sin_2-sin_1))*self.dt
        for i in range(self.elementAmount-1):
            self.x[i] += self.v_x[i]*self.dt
            self.y[i] += self.v_y[i]*self.dt



    def plotChain(self):
        self.updateCoords()
        self.count+=1
        if self.count==int(1/self.dt/self.fps):
            sc = MplCanvas(width=5, height=4, dpi=100)
            self.layout.addWidget(sc,0,0)
            sc.axes.plot(self.x, self.y, marker='o')
            sc.axes.set_title('модель цепочки')
            sc.axes.set()
            self.x_plot = self.getStartingXs()
            self.y_plot = []
            for i in range(self.elementAmount):
                self.y_plot.append(self.getStartingYs(self.x_plot[i]))
            sc.axes1.plot(self.x_plot, self.v_x, marker='o')
            sc.axes1.set(xlim=(0,self.x_plot[-1]), ylim=(-0.3,0.3))
            sc.axes1.set_title('горизонтальные скорости')
            sc.axes2.plot(self.x_plot, self.v_y, marker='o')
            sc.axes2.set(xlim=(0,self.x_plot[-1]), ylim=(-0.025,0.025))
            sc.axes2.set_title('вертикальные скорости')
            sc.axes.set_xlabel('x')
            sc.axes.set_ylabel('y')
            sc.axes1.set_xlabel('x')
            sc.axes1.set_ylabel('v_x')
            sc.axes2.set_xlabel('x')
            sc.axes2.set_ylabel('v_y')

            
            self.label.setText("Current time: "+str(round(self.current_time,1)))
            
            self.show()
            self.count = 0
        self.current_time+=self.dt

    def createModelWindow(self):
        #self.m = float(self.input_1.text())
        #self.a = float(self.input_2.text())
        #self.c = float(self.input_3.text())
        self.c = 100
        self.a = self.x_step
        self.m = 0.1
        self.dt = pow((self.m/self.c) ,0.5)/10
        print(self.dt)
        for i in reversed(range(self.layout.count())): 
            self.layout.itemAt(i).widget().setParent(None)
        sc = MplCanvas(width=5, height=4, dpi=100)
        self.layout.addWidget(sc,0,0)
        sc.axes.plot(self.x, self.y, marker='o')
        sc.axes.set_title('модель цепочки')
        self.x_plot = self.getStartingXs()
        self.y_plot = []
        for i in range(self.elementAmount):
            self.y_plot.append(self.getStartingYs(self.x_plot[i]))
        sc.axes1.plot(self.v_x, self.y_plot, marker='o')
        sc.axes1.set(xlim=(-0.3,0.3), ylim=(-0.3,0.3))
        sc.axes1.set_title('горизонтальные скорости')
        sc.axes2.plot(self.x_plot, self.v_y, marker='o')
        sc.axes2.set(xlim=(-0.3,0.3), ylim=(-0.3,0.3))
        sc.axes2.set_title('вертикальные скорости')
        sc.axes.set_xlabel('x')
        sc.axes.set_ylabel('y')
        sc.axes1.set_xlabel('x')
        sc.axes1.set_ylabel('v_x')
        sc.axes2.set_xlabel('x')
        sc.axes2.set_ylabel('v_y')

        self.layout.addWidget(self.label,1,0)
        self.label.setText("Current time: "+str(round(self.current_time,1)))
        
        self.show()
        self.timer=QTimer()
        self.timer.timeout.connect(self.plotChain)
        self.timer.start(int(1000*self.dt/self.speed_mod))

if __name__ == '__main__':
    app=QApplication(sys.argv)
    form=WinForm()
    form.show()
    sys.exit(app.exec_())
