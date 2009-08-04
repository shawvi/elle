//
// ---------- header ----------------------------------------------------------
//
// project       infinit
//
// license       infinit (c)
//
// file          /home/mycure/infinit/etoile/core/Catalog.hh
//
// created       julien quintard   [mon jul 27 10:19:21 2009]
// updated       julien quintard   [tue aug  4 14:02:12 2009]
//

#ifndef ETOILE_COMPONENTS_CATALOG_HH
#define ETOILE_COMPONENTS_CATALOG_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <etoile/core/ContentHashBlock.hh>

#include <list>

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
    class Catalog:
      public ContentHashBlock
    {
    public:
      //
      // structures
      //

      ///
      /// this structure defines a catalog entry.
      ///
      struct		Entry
      {
	String		name;
	Address		address;
      };

      //
      // types
      //
      typedef std::list<Entry*>		Container;

      typedef Container::iterator	Iterator;
      typedef Container::const_iterator	ConstIterator;

      //
      // methods
      //
      Status		Size(Natural32&);
      Status		Add(const String&,
			    const Address&);
      Status		Remove(const String&);
      Status		Lookup(const String&,
			       Address&);
      Status		Search(const String&,
			       Iterator* = NULL);

      //
      // constructors & destructors
      //
      ~Catalog();

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

      Container		entries;
    };

  }
}

#endif
