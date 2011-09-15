//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// author        julien quintard   [thu aug 11 10:44:19 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <elle/utility/Duration.hh>

namespace elle
{
  namespace utility
  {

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// default constructor.
    ///
    Duration::Duration():
      unit(Duration::UnitUnknown),
      value(0)
    {
    }

    ///
    /// common constructor.
    ///
    Duration::Duration(const Unit				unit,
		       const Natural64				value):
      unit(unit),
      value(value)
    {
    }

//
// ---------- object ----------------------------------------------------------
//

    ///
    /// this operator compares two objects.
    ///
    Boolean		Duration::operator==(const Duration&	element) const
    {
      enter();

      // compare the attributes.
      if ((this->unit != element.unit) ||
	  (this->value != element.value))
	false();

      true();
    }

    ///
    /// this macro-function call generates the object.
    ///
    embed(Duration, _());

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this function dumps an duration object.
    ///
    Status		Duration::Dump(Natural32		margin) const
    {
      String		alignment(margin, ' ');
 
      enter();

      std::cout << alignment << "[Duration]" << std::endl;

      std::cout << alignment << Dumpable::Shift << "[Unit] "
		<< std::nouppercase << std::dec	
		<< this->unit << std::endl;
      std::cout << alignment << Dumpable::Shift << "[Value] "
		<< std::nouppercase << std::dec	
		<< this->value << std::endl;

      leave();
    }

//
// ---------- archivable ------------------------------------------------------
//

    ///
    /// this method serializes the duration object.
    ///
    Status		Duration::Serialize(Archive&		archive) const
    {
      enter();

      // serialize the internal attributes.
      if (archive.Serialize(static_cast<Natural8>(this->unit),
			    this->value) == StatusError)
	escape("unable to serialize the attributes");

      leave();
    }

    ///
    /// this method extracts the duration object.
    ///
    Status		Duration::Extract(Archive&		archive)
    {
      enter();

      // extract the internal attributes.
      if (archive.Extract(reinterpret_cast<Natural8&>(this->unit),
			  this->value) == StatusError)
	escape("unable to extract the attributes");

      leave();
    }

  }
}
