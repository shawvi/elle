//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// author        julien quintard   [wed feb  3 16:49:46 2010]
//

//
// ---------- includes --------------------------------------------------------
//

#include <elle/network/Network.hh>
#include <elle/concurrency/Event.hh>

namespace elle
{
  namespace network
  {

//
// ---------- definitions -----------------------------------------------------
//

    ///
    /// this container holds the list of registered procedures.
    ///
    Network::Container                  Network::Procedures;

//
// ---------- static methods --------------------------------------------------
//

    ///
    /// this method initializes the network components.
    ///
    Status              Network::Initialize()
    {
      // initialize the session.
      if (Session::Initialize() == StatusError)
        escape("unable to initialize the session");

      return StatusOk;
    }

    ///
    /// this method cleans the network components.
    ///
    Status              Network::Clean()
    {
      // clean the session.
      if (Session::Clean() == StatusError)
        escape("unable to clean the session");

      Network::Procedures.clear();

      return StatusOk;
    }

    ///
    /// this method dumps the procedures.
    ///
    Status              Network::Show(const Natural32           margin)
    {
      String            alignment(margin, ' ');
      Network::Scoutor  scoutor;

      std::cout << alignment << "[Network]" << std::endl;

      return StatusOk;
    }

    Status
    Network::Register(Tag i, const Function& f)
    {
      assert(f);
      std::pair<Network::Iterator, Boolean>     result;

      // check if this tag has already been recorded.
      if (Network::Procedures.find(i) != Network::Procedures.end())
        escape("this tag seems to have already been recorded");
      result = Network::Procedures.insert
        (std::pair<const Tag, Function>(i, f));

      // check if the insertion was successful.
      if (result.second == false)
        escape("unable to insert the selectoinoid in the container");
      // FIXME: duplicate, remove
      assert(Network::Procedures.find(i)->second);

      return StatusOk;
    }
  }
}
