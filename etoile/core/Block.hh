//
// ---------- header ----------------------------------------------------------
//
// project       infinit
//
// license       infinit (c)
//
// file          /home/mycure/infinit/etoile/core/Block.hh
//
// created       julien quintard   [mon feb 16 18:47:31 2009]
// updated       julien quintard   [tue aug  4 13:55:11 2009]
//

#ifndef ETOILE_COMPONENTS_BLOCK_HH
#define ETOILE_COMPONENTS_BLOCK_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <etoile/core/Address.hh>

namespace etoile
{
  namespace core
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// XXX
    ///
    class Block:
      public Dumpable, public Archivable
    {
    public:
      //
      // methods
      //
      virtual Status	Seal() = 0;

      virtual Status	Validate(const Address&);

      //
      // interfaces
      //

      // dumpable
      Status		Dump(const Natural32 = 0) const;

      // archivable
      Status		Serialize(Archive&) const;
      Status		Extract(Archive&);

      //
      // attributes
      //

      ///
      /// this address is not serialized.
      ///
      Address		address;
    };

  }
}

#endif
