//
// ---------- header ----------------------------------------------------------
//
// project       hole
//
// license       infinit
//
// author        julien quintard   [sun aug 28 17:53:05 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <hole/implementations/remote/Customer.hh>
#include <hole/implementations/remote/Remote.hh>

#include <Infinit.hh>

namespace hole
{
  namespace implementations
  {
    namespace remote
    {

//
// ---------- definitions -----------------------------------------------------
//

      ///
      /// this constant defines the time after which a customer should
      /// have authenticated.
      ///
      const elle::Natural32             Customer::Timeout = 180000;

//
// ---------- constructors & destructors --------------------------------------
//

      ///
      /// default constructor.
      ///
      Customer::Customer():
        state(Customer::StateUnknown),
        socket(NULL),
        timer(NULL)
      {
      }

      ///
      /// destructor.
      ///
      Customer::~Customer()
      {
        // delete the socket.
        if (this->socket != NULL)
          delete this->socket;

        // delete the timer.
        if (this->timer != NULL)
          delete this->timer;
      }

//
// ---------- methods ---------------------------------------------------------
//

      ///
      /// this method creates a customer from the given socket.
      ///
      elle::Status      Customer::Create(elle::TCPSocket*       socket)
      {
        // register the client.
        this->socket = socket;

        // subscribe to the signal.
        if (this->socket->signal.disconnected.Subscribe(
              elle::Callback<>::Infer(&Customer::Disconnected,
                                      this)) == elle::Status::Error)
          escape("unable to subscribe to the signal");

        // subscribe to the signal.
        if (this->socket->signal.error.Subscribe(
              elle::Callback<>::Infer(&Customer::Error,
                                      this)) == elle::Status::Error)
          escape("unable to subscribe to the signal");

        // allocate a new timer.
        this->timer = new elle::Timer;

        // create the timer.
        if (this->timer->Create(elle::Timer::ModeSingle) == elle::Status::Error)
          escape("unable to create the timer");

        // subscribe to the timer's signal.
        if (this->timer->signal.timeout.Subscribe(
              elle::Callback<>::Infer(&Customer::Abort,
                                      this)) == elle::Status::Error)
          escape("unable to subscribe to the signal");

        // start the timer.
        if (this->timer->Start(Customer::Timeout) == elle::Status::Error)
          escape("unable to start the timer");

        return elle::Status::Ok;
      }

//
// ---------- callbacks -------------------------------------------------------
//

      ///
      /// this callback is triggered whenever the customer is considered
      /// disconnected.
      ///
      elle::Status      Customer::Disconnected()
      {
        // debug.
        if (Infinit::Configuration.hole.debug == true)
          printf("[hole] implementations::remote::Customer::Disconnected()\n");

        // set the customer's state as dead.
        this->state = Customer::StateDead;

        // emit the signal.
        if (this->signal.dead.Emit(this) == elle::Status::Error)
          escape("unable to emit the signal");

        return elle::Status::Ok;
      }

      ///
      /// this callback is triggered whenever an error occurs on the
      /// customer's socket.
      ///
      elle::Status      Customer::Error(const elle::String&     error)
      {
        // debug.
        if (Infinit::Configuration.hole.debug == true)
          printf("[hole] implementations::remote::Customer::Error()\n");

        // log the error.
        log(error.c_str());

        // disconnect the socket, though that may be unecessary.
        this->socket->Disconnect();

        return elle::Status::Ok;
      }

      ///
      /// this callback is triggered once the authentication timer times out.
      ///
      /// if the customer has not been authenticated, it is destroyed.
      ///
      elle::Status      Customer::Abort()
      {
        // debug.
        if (Infinit::Configuration.hole.debug == true)
          printf("[hole] implementations::remote::Customer::Abort()\n");

        // delete the timer.
        delete this->timer;

        // reset the pointer.
        this->timer = NULL;

        // check if the customer has been authenticated.
        if (this->state != Customer::StateAuthenticated)
          {
            // emit the signal.
            if (this->signal.dead.Emit(this) == elle::Status::Error)
              escape("unable to emit the signal");
          }

        return elle::Status::Ok;
      }

//
// ---------- dumpable --------------------------------------------------------
//

      ///
      /// this method dumps the customer.
      ///
      elle::Status      Customer::Dump(const elle::Natural32    margin) const
      {
        elle::String    alignment(margin, ' ');

        std::cout << alignment << "[Customer]" << std::endl;

        // dump the state.
        std::cout << alignment << elle::Dumpable::Shift
                  << "[State] " << this->state << std::endl;

        // dump the socket.
        if (this->socket != NULL)
          {
            if (this->socket->Dump(margin + 2) == elle::Status::Error)
              escape("unable to dump the socket");
          }
        else
          {
            std::cout << alignment << elle::Dumpable::Shift
                      << "[TCPSocket] " << elle::none << std::endl;
          }

        // dump the timer.
        if (this->timer != NULL)
          {
            if (this->timer->Dump(margin + 2) == elle::Status::Error)
              escape("unable to dump the timer");
          }
        else
          {
            std::cout << alignment << elle::Dumpable::Shift
                      << "[Timer] " << elle::none << std::endl;
          }

        return elle::Status::Ok;
      }

    }
  }
}
