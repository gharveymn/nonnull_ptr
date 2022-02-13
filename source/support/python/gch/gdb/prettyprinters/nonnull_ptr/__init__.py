import gdb

def register_printers(obj):
  from .prettyprinter import gch_nonnull_ptr_printer
  gdb.printing.register_pretty_printer(obj, gch_nonnull_ptr_printer)

register_printers(None)
