#pragma once

#include <Automaton.h>
#include <RCSwitch.h>

class MySensorsRFReceiver: public Machine {

 public:
  enum { IDLE, ACCEPT, IGNORE }; // STATES
  enum { EVT_RECEIVE, EVT_TIMER, ELSE }; // EVENTS
  MySensorsRFReceiver( void ) : Machine() {};
  MySensorsRFReceiver& begin(uint8_t pin, uint32_t timeout);
  MySensorsRFReceiver& trace( Stream & stream );
  MySensorsRFReceiver& trigger( int event );
  int state( void );
  MySensorsRFReceiver& onReceive( Machine& machine, int event = 0 );
  MySensorsRFReceiver& onReceive( atm_cb_push_t callback, int idx = 0 );
  MySensorsRFReceiver& receive( void );
  uint32_t receivedValue;

 private:
  enum { ENT_ACCEPT, ENT_IGNORE, LOOP_IGNORE }; // ACTIONS
  enum { ON_RECEIVE, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id );
  void action( int id );
  RCSwitch rc;
  atm_timer_millis timeout_timer;
};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="MySensorsRFReceiver">
    <states>
      <IDLE index="0">
        <EVT_RECEIVE>ACCEPT</EVT_RECEIVE>
      </IDLE>
      <ACCEPT index="1" on_enter="ENT_ACCEPT">
        <ELSE>IGNORE</ELSE>
      </ACCEPT>
      <IGNORE index="2">
        <EVT_RECEIVE>IGNORE</EVT_RECEIVE>
        <EVT_TIMER>IDLE</EVT_TIMER>
      </IGNORE>
    </states>
    <events>
      <EVT_RECEIVE index="0" access="PUBLIC"/>
      <EVT_TIMER index="1" access="PRIVATE"/>
    </events>
    <connectors>
      <RECEIVE autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
