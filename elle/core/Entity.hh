//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit (c)
//
// file          /home/mycure/infinit/elle/core/Entity.hh
//
// created       julien quintard   [sun feb 22 19:43:33 2009]
// updated       julien quintard   [thu mar 25 00:53:50 2010]
//

#ifndef ELLE_CORE_ENTITY_HH
#define ELLE_CORE_ENTITY_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/core/Boolean.hh>
#include <elle/core/Natural.hh>

#include <elle/misc/Status.hh>

namespace elle
{
  using namespace misc;

  namespace core
  {

//
// ---------- macro functions -------------------------------------------------
//

///
/// this macro generates the prototypes for the source code below.
///
#define EntityDeclare(_type_)						\
  _type_&		operator=(const _type_&);			\
									\
  Boolean		operator!=(const _type_&) const;		\
									\
  Status		Imprint(Natural32&) const;			\
									\
  Status		Clone(Entity*&) const;

///
/// this macro function makes it easy for classes to derive from Entity
/// as only a copy constructor and a == operator must be manually written.
/// everything else, assignment, != operator, sizeof are automatically
/// generated by the following.
///
#define EntityEmbed(_type_, _template_...)				\
  _template_								\
  _type_&	_type_::operator=(const _type_&		element)	\
  {									\
    enter();								\
									\
    if (this == &element)						\
      return (*this);							\
									\
    if (this->Recycle(&element) == StatusError)				\
      yield("unable to recycle the object", *this);			\
									\
    return (*this);							\
  }									\
									\
  _template_								\
  Boolean	_type_::operator!=(const _type_&	element) const	\
  {									\
    return (!(*this == element));					\
  }									\
									\
  _template_								\
  Status	_type_::Imprint(Natural32&		size) const	\
  {									\
    enter();								\
									\
    size = sizeof(_type_);						\
									\
    leave();								\
  }									\
									\
  _template_								\
  Status	_type_::Clone(Entity*&			element) const	\
  {									\
    enter();								\
									\
    element = new _type_(*this);					\
									\
    leave();								\
  }

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this interface must be implemented by every 'entity' ... in the
    /// elle library terms.
    ///
    class Entity
    {
    public:
      //
      // constructors and destructors
      //

      ///
      /// every class inheriting directly or indirectly the Entity interface
      /// must define a default constructor.
      ///
      /// note that nothing---especially no allocation---should be performed
      /// in this method but initializing the attributes to default values.
      ///
      Entity()
      {
      }

      ///
      /// the copy constructor is responsible for duplicating an entity
      /// and should therefore be provided for every class also providing
      /// the = operator.
      ///
      Entity(const Entity&)
      {
      }

      ///
      /// this destructor should release the allocated resources but must
      /// not re-set attributes to default values.
      ///
      virtual ~Entity()
      {
      }

      //
      // methods
      //

      ///
      /// this method recycles an entity by deallocating resources and
      /// re-setting them to their default values.
      ///
      template <typename T>
      Status		Recycle(const T*			entity = NULL)
      {
	// release the resources.
	this->~Entity();

	if (entity == NULL)
	  {
	    // initialize the object with default values.
	    new (this) T;
	  }
	else
	  {
	    // initialize the object with defined values.
	    new (this) T(*entity);
	  }

	// return StatusOk in order to avoid including Report, Status and Maid.
	return (StatusOk);
      }

      virtual Status	Imprint(Natural32&) const;
      virtual Status	Clone(Entity*&) const;

      //
      // operators
      //
      virtual Entity&	operator=(const Entity&);
      virtual Boolean	operator==(const Entity&) const;
      virtual Boolean	operator!=(const Entity&) const;
    };

  }
}

#endif
