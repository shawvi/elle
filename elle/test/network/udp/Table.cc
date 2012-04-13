//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// author        julien quintard   [wed mar 17 13:23:40 2010]
//

//
// ---------- includes --------------------------------------------------------
//

#include <elle/test/network/udp/Table.hh>

namespace elle
{
  namespace test
  {

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method creates the table.
    ///
    Status              Table::Create(Node*                     node)
    {
      // set the node.
      this->node = node;

      // create a timer.
      if (this->timer.Create(Timer::ModeRepetition) == Status::Error)
        escape("unable to create the timer");

      // subscribe to the timer's signal.
      if (this->timer.signal.timeout.Subscribe(
            Callback<>::Infer(&Table::Renew, this)) == Status::Error)
        escape("unable to subscribe to the signal");

      // start the timer.
      if (this->timer.Start(Node::Rate) == Status::Error)
        escape("unable to start the timer");

      return Status::Ok;
    }

    ///
    /// this method adds/updates a neighbour.
    ///
    Status              Table::Update(const Locus&              locus,
                                      const String&             name)
    {
      Table::Iterator   iterator;

      // try to locate a previous entry.
      if (this->Locate(locus, iterator) == Status::Ok)
        {
          // update the neighbour.
          if ((*iterator)->Update(name) == Status::Error)
            escape("unable to refresh the node");
        }
      else
        {
          Neighbour*    neighbour;

          // allocate a new neighbour.
          neighbour = new Neighbour;

          // create a new neighbour.
          if (neighbour->Create(this->node, locus, name) == Status::Error)
            escape("unable to create the neighbour");

          // add the neighbour to the list.
          this->container.push_back(neighbour);
        }

      return Status::Ok;
    }

    ///
    /// this method refreshes an entry.
    ///
    Status              Table::Refresh(const Locus&             locus)
    {
      Table::Iterator   iterator;

      // locate the neighbour.
      if (this->Locate(locus, iterator) != Status::Ok)
        escape("unable to locate the given locus");

      // refresh the neighbour.
      if ((*iterator)->Refresh() == Status::Error)
        escape("unable to refresh the node");

      return Status::Ok;
    }

    ///
    /// this method removes a neighbour.
    ///
    Status              Table::Remove(const Locus&              locus)
    {
      Table::Iterator   iterator;

      // try to locate a previous entry.
      if (this->Locate(locus, iterator) == Status::Error)
        escape("unable to locate this neighbour");

      // delete the neighbour.
      delete *iterator;

      // remove the element from the list.
      this->container.erase(iterator);

      return Status::Ok;
    }

    ///
    /// this method locates a neighbour in the list.
    ///
    Status              Table::Locate(const Locus&              locus,
                                      Table::Iterator&          iterator)
    {
      // iterator over the container.
      for (iterator = this->container.begin();
           iterator != this->container.end();
           iterator++)
        {
          // if the locus is found, return.
          if ((*iterator)->locus == locus)
            return Status::Ok;
        }

      escape("unable to locate the given neighbour");
    }

    ///
    /// this method merges two table.
    ///
    Status              Table::Merge(const Table&               table)
    {
      Table::Scoutor    scoutor;

      // explore the received table.
      for (scoutor = table.container.begin();
           scoutor != table.container.end();
           scoutor++)
        {
          // add/update the table.
          if (this->Update((*scoutor)->locus,
                           (*scoutor)->name) == Status::Error)
            escape("unable to update the table");
        }

      return Status::Ok;
    }

//
// ---------- archivable ------------------------------------------------------
//

    ///
    /// this method serializes a table.
    ///
    Status              Table::Serialize(Archive&               archive) const
    {
      Table::Scoutor    scoutor;

      // serialize the number of neighbours.
      if (archive.Serialize(static_cast<Natural32>(this->container.size())) ==
          Status::Error)
        escape("unable to serialize the neighbour");

      // serialize the table by going through it.
      for (scoutor = this->container.begin();
           scoutor != this->container.end();
           scoutor++)
        {
          // serialize the neighbour.
          if (archive.Serialize(*(*scoutor)) == Status::Error)
            escape("unable to serialize the neighbour");
        }

      return Status::Ok;
    }

    ///
    /// this method extracts a table.
    ///
    Status              Table::Extract(Archive&         archive)
    {
      Natural32         n;
      Natural32         i;

      // extract the number of elements.
      if (archive.Extract(n) == Status::Error)
        escape("unable to extract the internal numbers");

      // iterate.
      for (i = 0; i < n; i++)
        {
          Neighbour*    neighbour;

          // allocate a neighbour.
          neighbour = new Neighbour;

          // extract the neighbour.
          if (archive.Extract(*neighbour) == Status::Error)
            escape("unable to extract the neighbour");

          // add the neighbour to the table.
          this->container.push_back(neighbour);
        }

      return Status::Ok;
    }

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method dumps the table.
    ///
    Status              Table::Dump(const Natural32             margin) const
    {
      String            alignment(margin, ' ');
      Table::Scoutor    scoutor;

      std::cout << alignment << "[Table]" << std::endl;

      for (scoutor = this->container.begin();
           scoutor != this->container.end();
           scoutor++)
        {
          // dump the neighbour.
          if ((*scoutor)->Dump(margin + 2) == Status::Error)
            escape("unable to dump the neighbour");
        }

      return Status::Ok;
    }

//
// ---------- callbacks -------------------------------------------------------
//

    ///
    /// this callback is called whenever the state needs refreshing.
    ///
    Status              Table::Renew()
    {
      Table::Scoutor    scoutor;

      //
      // first, dump the table.
      //
      if (this->Dump() == Status::Error)
        escape("unable to dump the table");

      //
      // initiate the refreshing process.
      //
      for (scoutor = this->container.begin();
           scoutor != this->container.end();
           scoutor++)
        {
          // send a probe message.
          if (this->node->socket.Send(
                (*scoutor)->locus,
                Inputs<TagProbe>(this->node->name,
                                 this->node->table)) == Status::Error)
              escape("unable to send a probe");
        }

      return Status::Ok;
    }

  }
}
