# -*- coding: utf-8 -*-

from tkinter import *
from tkinter import messagebox, ttk
from typing import List, Optional, Tuple

import MultiListbox as table


Record = List[str]
Field = Tuple[str, StringVar]


data: List[Record] = [
    [
        "Petr",
        "Bílý",
        "045214/1512",
        "17. listopadu",
        "15",
        "Ostrava",
        "70800",
        "poznamka",
    ],
    ["Jana", "Zelený", "901121/7238", "Vozovna", "54", "Poruba", "78511", ""],
    ["Karel", "Modrý", "800524/5417", "Porubská", "7", "Praha", "11150", ""],
    ["Martin", "Stříbrný", "790407/3652", "Sokolovská", "247", "Brno", "54788", "nic"],
]


class App:
    def __init__(self, root: Tk) -> None:
        self.root = root
        self.selected_row: Optional[int] = None

        self.jmeno = StringVar()
        self.prijmeni = StringVar()
        self.rc = StringVar()
        self.ulice = StringVar()
        self.cp = StringVar()
        self.mesto = StringVar()
        self.psc = StringVar()
        self.mlb: table.MultiListbox
        self.nb: ttk.Notebook
        self.p1: Frame
        self.p2: Frame
        self.note_text: Text

        self.create_menu()
        self.create_table()
        self.create_identity_form()
        self.create_tabs()
        self.create_buttons()

    def create_menu(self) -> None:
        menubar = Menu(self.root)

        filemenu = Menu(menubar, tearoff=0)
        filemenu.add_command(label="Nový záznam", command=self.new_record)
        filemenu.add_separator()
        filemenu.add_command(label="Konec", command=self.root.quit)
        menubar.add_cascade(label="Soubor", menu=filemenu)

        settingsmenu = Menu(menubar, tearoff=0)
        settingsmenu.add_command(label="Vymazat formulář", command=self.clear_form)
        menubar.add_cascade(label="Nastavení", menu=settingsmenu)

        helpmenu = Menu(menubar, tearoff=0)
        helpmenu.add_command(label="O aplikaci", command=self.show_about)
        menubar.add_cascade(label="Nápověda", menu=helpmenu)

        self.root.config(menu=menubar)

    def create_table(self) -> None:
        self.mlb = table.MultiListbox(
            self.root,
            (("First Name", 20), ("Last Name", 20), ("Birth Number", 20)),
        )
        self.mlb.pack(expand=YES, fill=BOTH, padx=10, pady=(10, 0))
        self.mlb.subscribe(self.edit)

        for listbox in self.mlb.lists:
            listbox.configure(height=10)

        self.refresh_table()

        ttk.Separator(self.root, orient=HORIZONTAL).pack(fill=X, padx=10, pady=(0, 0))

    def create_identity_form(self) -> None:
        form_frame = Frame(self.root)
        form_frame.pack(fill=X, padx=10, pady=8)

        fields: Tuple[Field, ...] = (
            ("Jméno:", self.jmeno),
            ("Příjmení:", self.prijmeni),
            ("Rodné číslo:", self.rc),
        )

        for row, (label, variable) in enumerate(fields):
            Label(form_frame, text=label, width=12, anchor="e").grid(
                row=row, column=0, padx=(0, 10), pady=3, sticky="e"
            )
            Entry(form_frame, textvariable=variable, width=22).grid(
                row=row, column=1, pady=3, sticky="w"
            )

        form_frame.grid_columnconfigure(0, weight=1)
        form_frame.grid_columnconfigure(1, weight=1)

    def create_tabs(self) -> None:
        self.nb = ttk.Notebook(self.root)
        self.p1 = Frame(self.nb)
        self.p2 = Frame(self.nb)

        self.nb.add(self.p1, text="Adresa")
        self.nb.add(self.p2, text="Poznámka")
        self.nb.pack(fill=BOTH, padx=10, pady=(0, 10))

        Label(self.p1, text="Adresa", anchor="w").pack(fill=X, padx=10, pady=(8, 4))

        address_form = Frame(self.p1)
        address_form.pack(fill=X, padx=10, pady=(0, 10))

        Label(address_form, text="Ulice:", width=12, anchor="e").grid(
            row=0, column=0, padx=(0, 10), pady=3, sticky="e"
        )
        Entry(address_form, textvariable=self.ulice, width=14).grid(
            row=0, column=1, pady=3, sticky="w"
        )
        Label(address_form, text="č.p.:", width=6, anchor="e").grid(
            row=0, column=2, padx=(10, 10), pady=3, sticky="e"
        )
        Entry(address_form, textvariable=self.cp, width=6).grid(
            row=0, column=3, pady=3, sticky="w"
        )

        Label(address_form, text="Město:", width=12, anchor="e").grid(
            row=1, column=0, padx=(0, 10), pady=3, sticky="e"
        )
        Entry(address_form, textvariable=self.mesto, width=30).grid(
            row=1, column=1, columnspan=3, pady=3, sticky="w"
        )

        Label(address_form, text="PSČ:", width=12, anchor="e").grid(
            row=2, column=0, padx=(0, 10), pady=3, sticky="e"
        )
        Entry(address_form, textvariable=self.psc, width=8).grid(
            row=2, column=1, pady=3, sticky="w"
        )

        address_form.grid_columnconfigure(4, weight=1)

        self.note_text = Text(self.p2, height=6, width=20)
        self.note_text.pack(expand=YES, fill=BOTH, padx=10, pady=10)

    def create_buttons(self) -> None:
        button_frame = Frame(self.root)
        button_frame.pack(pady=(0, 10))

        Button(button_frame, text="Cancel", width=12, command=self.cancel_edit).pack(
            side=LEFT, padx=4
        )
        Button(
            button_frame, text="Nový záznam", width=12, command=self.new_record
        ).pack(side=LEFT, padx=4)
        Button(
            button_frame, text="Uložit záznam", width=12, command=self.save_record
        ).pack(side=LEFT, padx=4)

    def refresh_table(self) -> None:
        self.mlb.delete(0, END)
        for record in data:
            self.mlb.insert(END, (record[0], record[1], record[2]))

    def clear_form(self) -> None:
        self.jmeno.set("")
        self.prijmeni.set("")
        self.rc.set("")
        self.ulice.set("")
        self.cp.set("")
        self.mesto.set("")
        self.psc.set("")
        self.note_text.delete("1.0", END)

    def new_record(self) -> None:
        self.selected_row = None
        self.mlb.selection_clear(0, END)
        self.clear_form()
        self.nb.select(self.p1)

    def edit(self, row: int) -> None:
        self.selected_row = row
        record = data[row]

        self.jmeno.set(record[0])
        self.prijmeni.set(record[1])
        self.rc.set(record[2])
        self.ulice.set(record[3])
        self.cp.set(record[4])
        self.mesto.set(record[5])
        self.psc.set(record[6])

        self.note_text.delete("1.0", END)
        self.note_text.insert("1.0", record[7])

    def cancel_edit(self) -> None:
        if self.selected_row is None:
            self.clear_form()
            return

        self.edit(self.selected_row)

    def save_record(self) -> None:
        record: Record = [
            self.jmeno.get().strip(),
            self.prijmeni.get().strip(),
            self.rc.get().strip(),
            self.ulice.get().strip(),
            self.cp.get().strip(),
            self.mesto.get().strip(),
            self.psc.get().strip(),
            self.note_text.get("1.0", END).strip(),
        ]

        if not record[0] or not record[1] or not record[2]:
            messagebox.showwarning(
                "Neúplný záznam", "Vyplňte prosím jméno, příjmení a rodné číslo."
            )
            return

        if self.selected_row is None:
            data.append(record)
            self.selected_row = len(data) - 1
        else:
            data[self.selected_row] = record

        self.refresh_table()
        self.mlb.selection_set(self.selected_row)
        self.mlb.see(self.selected_row)
        self.edit(self.selected_row)

    def show_about(self) -> None:
        messagebox.showinfo("Nápověda", "Ukázkový formulář pro editaci osobních údajů.")


root = Tk()
root.wm_title("Formulář")
root.geometry("760x620")
root.minsize(700, 560)
app = App(root)
root.mainloop()
