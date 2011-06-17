//
// ---------- header ----------------------------------------------------------
//
// project       nucleus
//
// license       infinit
//
// file          /home/mycure/infinit/nucleus/proton/Location.cc
//
// created       julien quintard   [wed jun 15 20:17:21 2011]
// updated       julien quintard   [wed jun 15 20:55:11 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <nucleus/proton/Location.hh>

namespace nucleus
{
  namespace proton
  {

//
// ---------- definitions -----------------------------------------------------
//

    ///
    /// this constant represents a null location.
    ///
    const Location		Location::Null;

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// default constructor.
    ///
    Location::Location()
    {
    }

    ///
    /// specific constructor.
    ///
    Location::Location(const Address&				address,
		       const Version&				version):
      address(address),
      version(version)
    {
    }

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method creates a location.
    ///
    elle::Status	Location::Create(const Address&		address,
					 const Version&		version)
    {
      enter();

      // set the attributes.
      this->address = address;
      this->version = version;

      leave();
    }

//
// ---------- object ----------------------------------------------------------
//

    ///
    /// this method checks if two objects match.
    ///
    elle::Boolean	Location::operator==(const Location&	element) const
    {
      enter();

      // check the address as this may actually be the same object.
      if (this == &element)
	true();

      // compare the attributes.
      if ((this->address != element.address) ||
	  (this->version != element.version))
	false();

      true();
    }

    ///
    /// this macro-function call generates the object.
    ///
    embed(Location, _());

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method dumps the location's internals.
    ///
    elle::Status	Location::Dump(const elle::Natural32	margin) const
    {
      elle::String	alignment(margin, ' ');

      enter();

      std::cout << alignment << "[Location]" << std::endl;

      // dump the address.
      if (this->address.Dump(margin + 2) == elle::StatusError)
	escape("unable to dump the address");

      // dump the version.
      if (this->version.Dump(margin + 2) == elle::StatusError)
	escape("unable to dump the version");

      leave();
    }

//
// ---------- archivable ------------------------------------------------------
//

    ///
    /// this method serializes the location attributes.
    ///
    elle::Status	Location::Serialize(elle::Archive&	archive) const
    {
      enter();

      // serialize the attributes.
      if (archive.Serialize(this->address,
			    this->version) == elle::StatusError)
	escape("unable to serialize the location's attributes");

      leave();
    }

    ///
    /// this method extracts the attributes.
    ///
    elle::Status	Location::Extract(elle::Archive&	archive)
    {
      enter();

      // extracts the attributes.
      if (archive.Extract(this->address,
			  this->version) == elle::StatusError)
	escape("unable to extract the location's attributes");

      leave();
    }

  }
}
