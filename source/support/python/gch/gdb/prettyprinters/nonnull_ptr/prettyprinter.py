import gdb

class GCHNonnullPtrPrinter(object):
  'Print a gch::nonnull_ptr'

  def __init__(self, val):
    self.val = val
    self.ptr = val['m_ptr']

  class _iterator(object):
    def __init__(self, ptr):
      self.ptr = ptr

    def __iter__(self):
      return self

    def __next__(self):
      if self.ptr is None:
        raise StopIteration
      curr = self.ptr
      self.ptr = None
      return ('[pointer]', curr)

  def children(self):
    return self._iterator(self.ptr)

  def to_string(self):
    return f'{self.val.type}'

  def display_hint(self):
    return None

def build_printer():
  pp = gdb.printing.RegexpCollectionPrettyPrinter('gch::nonnull_ptr')
  pp.add_printer('gch::nonnull_ptr', '^gch::nonnull_ptr<.*>$', GCHNonnullPtrPrinter)
  return pp

gch_nonnull_ptr_printer = build_printer()
