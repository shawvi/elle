//
// ---------- header ----------------------------------------------------------
//
// project       nucleus
//
// license       infinit
//
// author        julien quintard   [wed jul  6 09:21:17 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <nucleus/proton/History.hh>

#include <lune/Lune.hh>

namespace nucleus
{
  namespace proton
  {

//
// ---------- definitions -----------------------------------------------------
//

    ///
    /// this string defines the history files extension.
    ///
    const elle::String          History::Extension = ".hty";

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method adds a version to the history.
    ///
    elle::Status        History::Register(const Version&        version)
    {
      // store the version in the history's vector.
      this->container.push_back(version);

      return elle::Status::Ok;
    }

    ///
    /// this method returns the version object corresponding to the given
    /// index number.
    ///
    elle::Status        History::Select(const Version::Type     index,
                                        Version&                version) const
    {
      // check if the index is out of bound.
      if (index >= this->container.size())
        escape("the version index is out of bound");

      // return the version.
      version = this->container[index];

      return elle::Status::Ok;
    }

    ///
    /// this method returns the size of the history.
    ///
    elle::Status        History::Size(Version::Type&            size) const
    {
      // return the size.
      size = this->container.size();

      return elle::Status::Ok;
    }

//
// ---------- object ----------------------------------------------------------
//

    ///
    /// this operator compares two objects.
    ///
    elle::Boolean       History::operator==(const History&      element) const
    {
      Version::Type     size;
      Version::Type     i;

      // check the address as this may actually be the same object.
      if (this == &element)
        return elle::Status::True;

      // check the containers' size.
      if (this->container.size() != element.container.size())
        return elle::Status::False;

      // retrieve the size.
      size = this->container.size();

      // go through the container and compare.
      for (i = 0; i < size; i++)
        {
          // compare the containers.
          if (this->container[i] != element.container[i])
            return elle::Status::False;
        }

      return elle::Status::True;
    }

    ///
    /// this macro-function call generates the object.
    ///
    embed(History, _());

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this function dumps an history object.
    ///
    elle::Status        History::Dump(elle::Natural32           margin) const
    {
      elle::String      alignment(margin, ' ');
      Version::Type     i;

      // display the name.
      std::cout << alignment << "[History]" << std::endl;

      // go through the container.
      for (i = 0; i < this->container.size(); i++)
        {
          Version       version;

          // display the entry.
          std::cout << alignment << elle::Dumpable::Shift
                    << "[Entry]" << std::endl;

          // display the index.
          std::cout << alignment << elle::Dumpable::Shift
                    << "[Index] " << i << std::endl;

          // retrieve the version.
          version = this->container[i];

          // dump the version.
          if (version.Dump(margin + 4) == elle::Status::Error)
            escape("unable to dump the version");
        }

      return elle::Status::Ok;
    }

//
// ---------- archivable ------------------------------------------------------
//

    ///
    /// this method serializes the history object.
    ///
    elle::Status        History::Serialize(elle::Archive&       archive) const
    {
      Version::Type     size;
      Version::Type     i;

      // retrieve the size.
      size = this->container.size();

      // serialize the size.
      if (archive.Serialize(size) == elle::Status::Error)
        escape("unable to serialize the history's size");

      // go through the container.
      for (i = 0; i < this->container.size(); i++)
        {
          // serialize the version;
          if (archive.Serialize(this->container[i]) == elle::Status::Error)
            escape("unable to serialize the version");
        }

      return elle::Status::Ok;
    }

    ///
    /// this method extracts the history object.
    ///
    elle::Status        History::Extract(elle::Archive&         archive)
    {
      Version::Type     size;
      Version::Type     i;

      // extract the size.
      if (archive.Extract(size) == elle::Status::Error)
        escape("unable to extract the history's size");

      // go through the archive.
      for (i = 0; i < size; i++)
        {
          Version       version;

          // extract the version;
          if (archive.Extract(version) == elle::Status::Error)
            escape("unable to extract the version");

          // register the version.
          if (this->Register(version) == elle::Status::Error)
            escape("unable to register the version");
        }

      return elle::Status::Ok;
    }

//
// ---------- fileable --------------------------------------------------------
//

    ///
    /// this method loads a history.
    ///
    elle::Status        History::Load(const Network&            network,
                                      const Address&            address)
    {
      elle::Path        path;
      elle::String      unique;
      elle::Region      region;
      elle::Archive     archive;

      // first, turn the block's address into a hexadecimal string.
      if (elle::Hexadecimal::Encode(address.digest->region,
                                    unique) == elle::Status::Error)
        escape("unable to convert the address in its hexadecimal form");

      // create the shelter path.
      if (path.Create(lune::Lune::Network::Shelter::History) ==
          elle::Status::Error)
        escape("unable to create the path");

      // complete the path with the network name.
      if (path.Complete(elle::Piece("%NETWORK%", network.name),
                        elle::Piece("%ADDRESS%", unique)) == elle::Status::Error)
        escape("unable to complete the path");

      // read the file's content.
      if (elle::File::Read(path, region) == elle::Status::Error)
        escape("unable to read the file's content");

      // wrap the region into an archive.
      if (archive.Wrap(region) == elle::Status::Error)
        escape("unable to prepare the archive");

      // extract from the archive.
      if (archive.Extract(*this) == elle::Status::Error)
        escape("unable to extract the archive");

      return elle::Status::Ok;
    }

    ///
    /// this method stores the history in its file format.
    ///
    elle::Status        History::Store(const Network&           network,
                                       const Address&           address) const
    {
      elle::Path        path;
      elle::String      unique;
      elle::Region      region;
      elle::Archive     archive;

      // first, turn the block's address into a hexadecimal string.
      if (elle::Hexadecimal::Encode(address.digest->region,
                                    unique) == elle::Status::Error)
        escape("unable to convert the address in its hexadecimal form");

      // create the shelter path.
      if (path.Create(lune::Lune::Network::Shelter::History) ==
          elle::Status::Error)
        escape("unable to create the path");

      // complete the path with the network name.
      if (path.Complete(elle::Piece("%NETWORK%", network.name),
                        elle::Piece("%ADDRESS%", unique)) == elle::Status::Error)
        escape("unable to complete the path");

      // create the archive.
      if (archive.Create() == elle::Status::Error)
        escape("unable to create the archive");

      // serialize the object.
      if (archive.Serialize(*this) == elle::Status::Error)
        escape("unable to serialize the object");

      // wrap the string.
      if (region.Wrap(reinterpret_cast<const elle::Byte*>(archive.contents),
                      archive.size) == elle::Status::Error)
        escape("unable to wrap the archive in a region");

      // write the file's content.
      if (elle::File::Write(path, region) == elle::Status::Error)
        escape("unable to write the file's content");

      return elle::Status::Ok;
    }

    ///
    /// this method erases a block.
    ///
    elle::Status        History::Erase(const Network&           network,
                                       const Address&           address) const
    {
      elle::Path        path;
      elle::String      unique;

      // first, turn the block's address into a hexadecimal string.
      if (elle::Hexadecimal::Encode(address.digest->region,
                                    unique) == elle::Status::Error)
        escape("unable to convert the address in its hexadecimal form");

      // create the shelter path.
      if (path.Create(lune::Lune::Network::Shelter::History) ==
          elle::Status::Error)
        escape("unable to create the path");

      // complete the path with the network name.
      if (path.Complete(elle::Piece("%NETWORK%", network.name),
                        elle::Piece("%ADDRESS%", unique)) == elle::Status::Error)
        escape("unable to complete the path");

      // is the file present...
      if (elle::File::Exist(path) == elle::Status::True)
        {
          // erase the file.
          if (elle::File::Erase(path) == elle::Status::Error)
            escape("unable to erase the file");
        }

      return elle::Status::Ok;
    }

    ///
    /// this method returns true if the block exists.
    ///
    elle::Status        History::Exist(const Network&           network,
                                       const Address&           address) const
    {
      elle::Path        path;
      elle::String      unique;

      // first, turn the block's address into a hexadecimal string.
      if (elle::Hexadecimal::Encode(address.digest->region,
                                    unique) == elle::Status::Error)
        flee("unable to convert the address in its hexadecimal form");

      // create the shelter path.
      if (path.Create(lune::Lune::Network::Shelter::History) ==
          elle::Status::Error)
        flee("unable to create the path");

      // complete the path with the network name.
      if (path.Complete(elle::Piece("%NETWORK%", network.name),
                        elle::Piece("%ADDRESS%", unique)) == elle::Status::Error)
        flee("unable to complete the path");

      // test the file.
      if (elle::File::Exist(path) == elle::Status::True)
        return elle::Status::True;

      return elle::Status::False;
    }

  }
}
