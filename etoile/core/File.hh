//
// ---------- header ----------------------------------------------------------
//
// project       infinit
//
// license       infinit (c)
//
// file          /home/mycure/infinit/etoile/core/File.hh
//
// created       julien quintard   [sun mar  8 15:07:58 2009]
// updated       julien quintard   [tue aug  4 21:01:25 2009]
//

#ifndef ETOILE_COMPONENTS_FILE_HH
#define ETOILE_COMPONENTS_FILE_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <etoile/core/Object.hh>

namespace etoile
{
  namespace core
  {

//
// ---------- classes ---------------------------------------------------------
//

    class File:
      public Object
    {
    public:
      //
      // methods
      //
      Status		Create(const KeyPair&);
    };

  }
}

#endif
