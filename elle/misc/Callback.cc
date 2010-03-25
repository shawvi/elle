//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/misc/Callback.cc
//
// created       julien quintard   [wed mar 24 15:51:36 2010]
// updated       julien quintard   [thu mar 25 00:54:57 2010]
//

#ifndef ELLE_MISC_CALLBACK_HXX
#define ELLE_MISC_CALLBACK_HXX

//
// ---------- includes --------------------------------------------------------
//

#include <elle/misc/Callback.hh>

///
/// these includes are placed here in order to prevent pre-processing
/// conflicts
///
#include <elle/misc/Maid.hh>
#include <elle/misc/Report.hh>

namespace elle
{
  namespace misc
  {

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// default constructor.
    ///
    Callback::Callback():
      routine(NULL)
    {
    }

    ///
    /// routine constructor.
    ///
    Callback::Callback(const Routine&			routine):
      routine(NULL)
    {
      enter();

      // clone the routine.
      if (routine.Clone((Entity*&)this->routine) == StatusError)
	alert("unable to clone the routine");

      release();
    }

    ///
    /// copy constructor.
    ///
    Callback::Callback(const Callback&			callback)
    {
      enter();

      // delete the previous routine.
      if (this->routine != NULL)
	delete this->routine;

      // allocate and copy the routine, if necessary.
      if (callback.routine != NULL)
	{
	  // clone the routine.
	  if (this->routine->Clone((Entity*&)this->routine) == StatusError)
	    alert("unable to clone the routine");
	}
      else
	{
	  // set the pointer to NULL.
	  this->routine = NULL;
	}

      release();
    }

    ///
    /// destructor.
    ///
    Callback::~Callback()
    {
      // release the routine.
      if (this->routine != NULL)
	delete this->routine;
    }

//
// ---------- entity ----------------------------------------------------------
//

    ///
    /// this macro-function call generates the entity.
    ///
    embed(Entity, Callback);

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method dumps the callback.
    ///
    Status		Callback::Dump(const Natural32		margin) const
    {
      String		alignment(margin, ' ');

      enter();

      std::cout << alignment << "[Callback]" << std::endl;

      // dump the routine.
      if (this->routine != NULL)
	if (this->routine->Dump(margin + 2) == StatusError)
	  escape("unable to dump the routine");

      leave();
    }

  }
}

#endif
