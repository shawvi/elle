//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// author        julien quintard   [wed feb  3 21:52:30 2010]
//

//
// ---------- includes --------------------------------------------------------
//

#include <elle/network/UDPSocket.hh>
#include <elle/network/Raw.hh>
#include <elle/network/Inputs.hh>
#include <elle/network/Network.hh>

namespace elle
{
  namespace network
  {

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// default constructor.
    ///
    UDPSocket::UDPSocket():
      Socket::Socket(Socket::TypeUDP),

      port(0),
      socket(NULL)
    {
    }

    ///
    /// the destructor releases the associated resources.
    ///
    UDPSocket::~UDPSocket()
    {
      // release the socket.
      if (this->socket != NULL)
        this->socket->deleteLater();
    }

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method creates a socket and connects the default signals.
    ///
    Status              UDPSocket::Create()
    {
      // allocate a new UDP socket.
      this->socket = new ::QUdpSocket;

      // bind the socket.
      if (this->socket->bind() == false)
        escape(this->socket->errorString().toStdString().c_str());

      // retrieve the port.
      this->port = this->socket->localPort();

      // subscribe to the signal.
      if (this->signal.ready.Subscribe(
            Callback<>::Infer(&UDPSocket::Dispatch, this)) == StatusError)
        escape("unable to subscribe to the signal");

      // connect the QT signals, depending on the mode.
      if (this->connect(this->socket, SIGNAL(readyRead()),
                        this, SLOT(_ready())) == false)
        escape("unable to connect the signal");

      if (this->connect(
            this->socket,
            SIGNAL(error(const QAbstractSocket::SocketError)),
            this,
            SLOT(_error(const QAbstractSocket::SocketError))) == false)
        escape("unable to connect to signal");

      return StatusOk;
    }

    ///
    /// this method creates a socket with a specific port.
    ///
    Status              UDPSocket::Create(const Port            port)
    {
      // set the port.
      this->port = port;

      // allocate a new UDP socket.
      this->socket = new ::QUdpSocket;

      // bind the socket to the port.
      if (this->socket->bind(this->port) == false)
        escape(this->socket->errorString().toStdString().c_str());

      // subscribe to the signal.
      if (this->signal.ready.Subscribe(
            Callback<>::Infer(&UDPSocket::Dispatch, this)) == StatusError)
        escape("unable to subscribe to the signal");

      // connect the QT signals.
      if (this->connect(this->socket, SIGNAL(readyRead()),
                        this, SLOT(Fetch())) == false)
        escape("unable to connect the signal");

      if (this->connect(
            this->socket,
            SIGNAL(error(const QAbstractSocket::SocketError)),
            this,
            SLOT(_error(const QAbstractSocket::SocketError))) == false)
        escape("unable to connect to signal");

      return StatusOk;
    }

    ///
    /// this method writes a packet on the socket so that it gets sent
    /// to the given locus.
    ///
    Status              UDPSocket::Write(const Locus&           locus,
                                         const Packet&          packet)
    {
      // push the datagram into the socket.
      if (this->socket->writeDatagram(
            reinterpret_cast<const char*>(packet.contents),
            packet.size,
            locus.host.location,
            locus.port) != static_cast<qint64>(packet.size))
        escape(this->socket->errorString().toStdString().c_str());

      return StatusOk;
    }

    ///
    /// this method reads datagram(s) from the socket.
    ///
    /// note that since UDP datagrams are constrained by a maxium size,
    /// there is no need to buffer the fetched datagram hoping for the next
    /// one to complete it.
    ///
    /// therefore, whenever a datagram is fetched, the method tries to makes
    /// sense out of it. if unable, the fetched datagrams are simply dropped.
    ///
    /// indeed there is no way to extract a following datagram if
    /// the first one is invalid since datagrams are variable in size.
    ///
    /// the method returns a raw with the read data.
    ///
    Status              UDPSocket::Read(Locus&                  locus,
                                        Raw&                    raw)
    {
      Natural32         size;

      // retrieve the size of the pending packet.
      size = this->socket->pendingDatagramSize();

      // check if there is data to be read.
      if (size == 0)
        return StatusOk;

      // set the locus as being an IP locus.
      if (locus.host.Create(Host::TypeIP) == StatusError)
        escape("unable to create an IP locus");

      // prepare the raw
      if (raw.Prepare(size) == StatusError)
        escape("unable to prepare the raw");

      // read the datagram from the socket.
      if (this->socket->readDatagram(
            reinterpret_cast<char*>(raw.contents),
            size,
            &locus.host.location,
            &locus.port) != size)
        escape(this->socket->errorString().toStdString().c_str());

      // set the raw's size.
      raw.size = size;

      return StatusOk;
    }

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method dumps the socket's state.
    ///
    Status              UDPSocket::Dump(const Natural32         margin) const
    {
      String            alignment(margin, ' ');

      std::cout << alignment << "[UDPSocket]" << std::endl;

      // dump the socket.
      if (Socket::Dump(margin + 2) == StatusError)
        escape("unable to dump the socket");

      return StatusOk;
    }

//
// ---------- callbacks -------------------------------------------------------
//

    ///
    /// this callback fetches parcels and dispatches them.
    ///
    Status              UDPSocket::Dispatch()
    {
      Locus             locus;
      Natural32         offset;
      Raw               raw;

      // read from the socket.
      if (this->Read(locus, raw) == StatusError)
        escape("unable to read from the socket");

      // initialize the offset.
      offset = 0;

      while (true)
        {
          Packet        packet;
          Region        frame;

          // check the remaining data.
          if ((raw.size - offset) == 0)
            break;

          // create the frame based on the previously extracted raw.
          if (frame.Wrap(raw.contents + offset,
                         raw.size - offset) == StatusError)
            escape("unable to wrap a frame in the raw");

          // prepare the packet based on the frame.
          if (packet.Wrap(frame) == StatusError)
            escape("unable to prepare the packet");

          // allocate the parcel.
          auto parcel = std::shared_ptr<Parcel>(new Parcel);

          // extract the header.
          if (parcel->header->Extract(packet) == StatusError)
            escape("unable to extract the header");

          // if there is not enough data in the raw to complete the parcel...
          if ((packet.size - packet.offset) < parcel->header->size)
            {
              log("this should not haappen in normal conditions");

              // exit the loop since there is not enough data anyway.
              break;
            }

          // extract the data.
          if (packet.Extract(*parcel->data) == StatusError)
            escape("unable to extract the data");

          // move to the next frame by setting the offset at the end of
          // the extracted frame.
          offset = offset + packet.offset;

          // create the session.
          if (parcel->session->Create(this,
                                      locus,
                                      parcel->header->event) == StatusError)
            escape("unable to create the session");

          // trigger the network shipment mechanism.
          if (Socket::Ship(parcel) == StatusError)
            log("an error occured while shipping the parcel");
        }

      return StatusOk;
    }

//
// ---------- slots -----------------------------------------------------------
//

    ///
    /// this slot is triggered when data is ready on the socket.
    ///
    void                UDPSocket::_ready()
    {
      new reactor::Thread(concurrency::scheduler(), "UDPSocket ready",
                          boost::bind(&Signal<Parameters<> >::Emit,
                                      &this->signal.ready), true);
    }

    ///
    /// this slot is triggered whenever an error occurs.
    ///
    /// note here that the type QLocalSocket::LocalSocketError cannot be
    /// written completely ::QLocalSocket::LocalSocketError because the
    /// QT parser is incapable of recognising the type.
    ///
    void                UDPSocket::_error(
                          const QAbstractSocket::SocketError)
    {
      String            cause(this->socket->errorString().toStdString());
      new reactor::Thread
        (concurrency::scheduler(), "UDPSocket error",
         boost::bind(&Signal<Parameters<const String& > >::Emit,
                     &this->signal.error, cause), true);
    }

  }
}
