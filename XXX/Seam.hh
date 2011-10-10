//
// ---------- header ----------------------------------------------------------
//
// project       nucleus
//
// license       infinit
//
// author        julien quintard   [mon sep 12 06:15:32 2011]
//

#ifndef NUCLEUS_PROTON_SEAM_HH
#define NUCLEUS_PROTON_SEAM_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <nucleus/proton/Address.hh>
#include <XXX/Nodule.hh>
#include <XXX/Quill.hh>
#include <XXX/Inlet.hh>

#include <elle/idiom/Close.hh>
# include <map>
#include <elle/idiom/Open.hh>

namespace nucleus
{
  namespace proton
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// XXX internal nodule
    ///
    template <typename V>
    class Seam<V>:
      public Nodule<V>
    {
    public:
      //
      // static attributes
      //
      static elle::Natural32	Footprint;

      //
      // static methods
      //
      static elle::Status	Initialize();
      static elle::Status	Clean();

      //
      // types
      //
      typedef Inlet< V, Nodule<V> >			I;

      //
      // types
      //
      typedef std::map<const typename V::K, I*>			Container;

      struct							Iterator
      {
	typedef typename Container::iterator			Forward;
	typedef typename Container::reverse_iterator		Backward;
      };

      struct							Scoutor
      {
	typedef typename Container::const_iterator		Forward;
	typedef typename Container::const_reverse_iterator	Backward;
      };

      //
      // constructors & destructors
      //
      Seam();
      Seam(const elle::Callback<
	     elle::Status,
	     elle::Parameters<
	       const Address&,
	       Nodule<V>*&
	       >
	     >&,
	   const elle::Callback<
	     elle::Status,
	     elle::Parameters<
	       const Address&,
	       const Nodule<V>*
	       >
	     >&);
      ~Seam();

      //
      // methods
      //
      elle::Status		Create();

      elle::Status		Insert(const typename V::K&,
				       Nodule<V>*);
      elle::Status		Insert(I*);

      elle::Status		Delete(typename Iterator::Forward&);
      elle::Status		Delete(Nodule<V>*);
      elle::Status		Delete(const typename V::K&);

      elle::Status		Lookup(const typename V::K&,
				       typename Iterator::Forward&);
      elle::Status		Lookup(const typename V::K&,
				       I*&);
      elle::Status		Lookup(const typename V::K&,
				       Nodule<V>*&);

      elle::Status		Locate(const typename V::K&,
				       typename Iterator::Forward&);
      elle::Status		Locate(const typename V::K&,
				       I*&);
      elle::Status		Locate(const typename V::K&,
				       Nodule<V>*&);

      elle::Status		Update(const typename V::K&,
				       const typename V::K&);
      elle::Status		Propagate(const typename V::K&,
					  const typename V::K&);

      elle::Status		Split(Seam<V>*&);
      elle::Status		Merge(Seam<V>*);

      //
      // interfaces
      //

      // nodule
      elle::Status		Major(typename V::K&) const;
      elle::Status		Search(const typename V::K&,
				       Quill<V>*&);

      // dumpable
      elle::Status		Dump(const elle::Natural32 = 0) const;

      // archivable
      elle::Status		Serialize(elle::Archive&) const;
      elle::Status		Extract(elle::Archive&);

      //
      // attributes
      //
      Container			container;
    };

  }
}

//
// ---------- templates -------------------------------------------------------
//

#include <XXX/Seam.hxx>

#endif
