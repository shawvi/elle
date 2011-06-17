//
// ---------- header ----------------------------------------------------------
//
// project       nucleus
//
// license       infinit
//
// file          /home/mycure/infinit/nucleus/proton/Stamp.hh
//
// created       julien quintard   [fri jun 17 13:44:59 2011]
// updated       julien quintard   [fri jun 17 14:54:17 2011]
//

#ifndef NUCLEUS_PROTON_HH
#define NUCLEUS_PROTON_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <nucleus/proton/Location.hh>

namespace nucleus
{
  namespace proton
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class represents a stamp issued by the oracle.
    ///
    /// a stamp states that a precise version of a mutable block i.e the
    /// master references another precise version of another mutable block
    /// i.e the slave.
    ///
    /// although this construct may sound useless it actually contributes
    /// greatly in ensuring security. for example stamps are used for
    /// linking an object with a group so that every vassal updating the
    /// object would need to acquire such a stamp from the oracle and then
    /// prove that she is a member of the linked group.
    ///
    /// thanks to the very light mechanism, vassals cannot lie regarding
    /// their memberships or permissions.
    ///
    class Stamp:
      public elle::Object
    {
    public:
      //
      // methods
      //
      elle::Status	Create(const Location&,
			       const Location&);

      elle::Status	Seal(const elle::PrivateKey&);
      elle::Status	Validate();

      //
      // interfaces
      //

      // object
      declare(Stamp);
      elle::Boolean	operator==(const Stamp&) const;

      // dumpable
      elle::Status	Dump(const elle::Natural32 = 0) const;

      // archivable
      elle::Status	Serialize(elle::Archive&) const;
      elle::Status	Extract(elle::Archive&);

      //
      // attributes
      //

      Location		master;
      Location		slave;

      elle::Signature	signature;
    };

  }
}

#endif
