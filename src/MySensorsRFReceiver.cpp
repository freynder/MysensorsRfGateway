#include "MySensorsRFReceiver.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

MySensorsRFReceiver& MySensorsRFReceiver::begin(uint8_t interrupt, uint32_t timeout) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*             ON_ENTER      ON_LOOP  ON_EXIT  EVT_RECEIVE  EVT_TIMER    ELSE */
    /*   IDLE */         -1,          -1,      -1,      ACCEPT,        -1,     -1,
    /* ACCEPT */ ENT_ACCEPT,          -1,      -1,          -1,        -1, IGNORE,
    /* IGNORE */         -1, LOOP_IGNORE,      -1,      IGNORE,      IDLE,     -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  timeout_timer.set(timeout);
  rc = RCSwitch();
  rc.enableReceive(interrupt);
  return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int MySensorsRFReceiver::event( int id ) {
  switch ( id ) {
    case EVT_RECEIVE:
      if(rc.available()) {
        if(state() == IGNORE) {
          // Throw away. This is necessary because the enter action is not
          // repeated when transitioning between identical states. It can thus
          // not be reset there.
          rc.resetAvailable();
        }
        return true;
      } else {
        return false;
      }
    case EVT_TIMER:
      return timeout_timer.expired(this);
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_RECEIVE, 0, <v>, <up> );
 */

void MySensorsRFReceiver::action( int id ) {
  switch ( id ) {
    case ENT_ACCEPT:
      receivedValue = rc.getReceivedValue();
      rc.resetAvailable();
      push(connectors, ON_RECEIVE, 0, 0, 0);
      return;
    case ENT_IGNORE:
      return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

MySensorsRFReceiver& MySensorsRFReceiver::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int MySensorsRFReceiver::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods
 *
 */

MySensorsRFReceiver& MySensorsRFReceiver::receive() {
  trigger( EVT_RECEIVE );
  return *this;
}

/*
 * onReceive() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

MySensorsRFReceiver& MySensorsRFReceiver::onReceive( Machine& machine, int event ) {
  onPush( connectors, ON_RECEIVE, 0, 1, 1, machine, event );
  return *this;
}

MySensorsRFReceiver& MySensorsRFReceiver::onReceive( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_RECEIVE, 0, 1, 1, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

MySensorsRFReceiver& MySensorsRFReceiver::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "MYSENSORSRFRECEIVER\0EVT_RECEIVE\0EVT_TIMER\0ELSE\0IDLE\0ACCEPT\0IGNORE" );
  return *this;
}
