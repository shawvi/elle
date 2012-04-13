//
// ---------- header ----------------------------------------------------------
//
// project       agent
//
// license       infinit
//
// author        julien quintard   [thu mar  4 17:51:46 2010]
//

//
// ---------- includes --------------------------------------------------------
//

#include <agent/Agent.hh>
#include <hole/Hole.hh>
#include <Infinit.hh>

namespace agent
{

//
// ---------- definitions -----------------------------------------------------
//

  ///
  /// this value defines the component's name.
  ///
  const elle::Character         Component[] = "agent";

  ///
  /// the user's identity.
  ///
  lune::Identity                Agent::Identity;

  ///
  /// this variable represents the user subject.
  ///
  nucleus::Subject              Agent::Subject;

//
// ---------- methods ---------------------------------------------------------
//

  ///
  /// this method initializes the agent.
  ///
  elle::Status          Agent::Initialize()
  {
    elle::String        prompt;
    elle::String        pass;

    // disable the meta logging.
    if (elle::Meta::Disable() == elle::Status::Error)
      escape("unable to disable the meta logging");

    //
    // load the identity.
    //
    {
      // does the identity exist.
      if (Agent::Identity.Exist() == elle::Status::False)
        escape("the user identity does not seem to exist");

      // prompt the user for the passphrase.
      prompt = "Enter passphrase for keypair '" + Infinit::User + "': ";

      if (elle::Console::Input(
            pass,
            prompt,
            elle::Console::OptionPassword) == elle::Status::Error)
        escape("unable to read the input");

      // load the identity.
      if (Agent::Identity.Load() == elle::Status::Error)
        escape("unable to load the identity");

      // verify the identity.
      if (Agent::Identity.Validate(Infinit::Authority) == elle::Status::Error)
        escape("the identity seems to be invalid");

      // decrypt the identity.
      if (Agent::Identity.Decrypt(pass) == elle::Status::Error)
        escape("unable to decrypt the identity");
    }

    //
    // create a subject representing the user.
    //
    {
      // create the subject.
      if (Agent::Subject.Create(Agent::Identity.pair.K) == elle::Status::Error)
        escape("unable to create the user's subject");
    }

    // enable the meta logging.
    if (elle::Meta::Enable() == elle::Status::Error)
      escape("unable to enable the meta logging");

    return elle::Status::Ok;
  }

  ///
  /// this method cleans the agent.
  ///
  /// the components are recycled just to make sure the memory is
  /// released before the Meta allocator terminates.
  ///
  elle::Status          Agent::Clean()
  {
    // nothing to do.

    return elle::Status::Ok;
  }

}
