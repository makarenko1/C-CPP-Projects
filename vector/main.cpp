#include <iostream>
#include "vl_vector.h"
#include <cassert>

int main ()
{
  vl_vector<int, 5> v;
  int *static_vec = v.data ();
  auto pos = v.begin ();
  for (int i = 5; i >= 0; --i)
    {
      assert (v.data () == static_vec);
      pos = v.insert (pos, i * 2);
      assert (pos == v.begin ());
    }

  assert (v.data () != static_vec);
  assert (v.capacity () == (size_t) (3 * 6) / 2);
  return 0;
}