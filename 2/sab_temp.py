# -*- coding: utf-8 -*-
import tkinter as tk
import tkinter.font as tkfont


class TemperatureConverter:
    def __init__(self, root):
        self.root = root
        self.root.title("tk")
        self.root.resizable(False, False)

        def_font = tkfont.nametofont("TkDefaultFont")
        def_font.config(size=14)

        self.left_frame = tk.Frame(root, padx=10, pady=10)
        self.left_frame.pack(side=tk.LEFT, fill=tk.Y)

        self.right_frame = tk.Frame(root, padx=10, pady=10)
        self.right_frame.pack(side=tk.RIGHT)

        self._build_direction_frame()
        self._build_io_frame()
        self._build_image_frame()

        self.root.bind("<Return>", self.convert)

    def _build_direction_frame(self):
        self.dir_var = tk.IntVar(value=1)

        self.dir_frame = tk.LabelFrame(self.left_frame, text="Směr převodu")
        self.dir_frame.pack(fill=tk.X, pady=(0, 10))

        self.rb_cf = tk.Radiobutton(
            self.dir_frame, text="C -> F", variable=self.dir_var, value=1
        )
        self.rb_cf.pack(side=tk.LEFT, expand=True, pady=5)

        self.rb_fc = tk.Radiobutton(
            self.dir_frame, text="F -> C", variable=self.dir_var, value=2
        )
        self.rb_fc.pack(side=tk.LEFT, expand=True, pady=5)

    def _build_io_frame(self):
        self.io_frame = tk.Frame(
            self.left_frame, relief=tk.GROOVE, borderwidth=1, padx=5, pady=5
        )
        self.io_frame.pack(fill=tk.BOTH, expand=True)

        self.lbl_in = tk.Label(self.io_frame, text="Input")
        self.lbl_in.pack()

        self.ent_in = tk.Entry(self.io_frame, width=15, justify="center")
        self.ent_in.pack(pady=(0, 10))
        self.ent_in.insert(0, "0")

        self.lbl_out = tk.Label(self.io_frame, text="Output")
        self.lbl_out.pack()

        self.ent_out = tk.Entry(self.io_frame, width=15, justify="center")
        self.ent_out.pack(pady=(0, 10))

        self.btn_convert = tk.Button(
            self.left_frame, text="Convert", command=self.convert
        )
        self.btn_convert.pack(pady=10)

        self.ent_in.focus_force()

    def _build_image_frame(self):
        self.ca = tk.Canvas(self.right_frame, width=150, height=350)
        self.ca.pack()
        self.photo = tk.PhotoImage(file="th.png")
        self.ca.create_image(75, 175, image=self.photo)

    def convert(self, event=None):
        try:
            val = float(self.ent_in.get())
            direction = self.dir_var.get()

            if direction == 1:
                res = (val * 9 / 5) + 32
            else:
                res = (val - 32) * 5 / 9

            self.ent_out.delete(0, tk.END)
            self.ent_out.insert(0, f"{res:.2f}")
        except ValueError:
            self.ent_out.delete(0, tk.END)
            self.ent_out.insert(0, "Invalid Input")


if __name__ == "__main__":
    root = tk.Tk()
    app = TemperatureConverter(root)
    root.mainloop()
