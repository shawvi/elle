//
// ---------- header ----------------------------------------------------------
//
// project       8network
//
// license       infinit
//
// file          /home/mycure/infinit/applications/8network/Network.hh
//
// created       julien quintard   [sat mar 27 08:37:14 2010]
// updated       julien quintard   [thu apr 22 22:20:20 2010]
//

#ifndef NETWORK_NETWORK_HH
#define NETWORK_NETWORK_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>
#include <etoile/Etoile.hh>

#include <elle/idiom/Close.hh>
# include <sys/stat.h>
#include <elle/idiom/Open.hh>

namespace application
{

//
// ---------- classes ---------------------------------------------------------
//

  ///
  /// this class implements the 8network application.
  ///
  class Network
  {
  public:
    //
    // constants
    //
    static const String			Path;

    //
    // static methods
    //
    static Status	New(const String&);
  };

}

#endif
