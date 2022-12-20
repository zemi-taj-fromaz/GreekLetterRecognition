from tkinter import *

WIDTH,HEIGHT = 500,500
CENTER = WIDTH//2

WHITE = (255,255,255)

class PaintGUI:
    def __init__(self,i):
        self.root = Tk()
        self.i = i
        self.root.title = "Draw a letter"
        self.coordinates = []
        self.brush_width = 15
        self.current_color = "#000000"
        self.cnv = Canvas(self.root, width =  WIDTH - 10, height = HEIGHT - 10, bg = "white")
        self.cnv.pack()
        self.cnv.bind("<B1-Motion>", self.paint)
        self.root.protocol("WM_DELETE_WINDOW",self.on_closing)
        self.root.attributes("-topmost",True)
        self.root.mainloop()

    def paint(self, event):
        self.coordinates.append((event.x,event.y))
        x1, y1 = (event.x - 1), (event.y - 1)
        x2, y2 = (event.x + 1), (event.y + 1)
        self.cnv.create_rectangle(x1,y1,x2,y2, fill = "white")
       # self.draw.rectangle([x1,y1,x2 + self.brush_width,y2+self.brush_width], outline = self.current_color,width=self.brush_width)

    def on_closing(self):
        f = open(f"test_data/greek_letter_{self.i}.txt","w")

        for i in range(len(self.coordinates)):
            output = str(self.coordinates[i][0]) + " " + str(self.coordinates[i][1]) + "\n"
            #if(i != len(self.coordinates) - 1):
             #   output+="\n"
            f.write(output)
            #print("[" + str(coordinate[0]) + " " + str(coordinate[1]) + "]")
        f.close()
        self.root.destroy()


for i in range(int(sys.argv[1])):
        PaintGUI(i)