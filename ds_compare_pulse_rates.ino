// Measure interval (period) of each of two pulse streams; compare to see if they're about the same;
// if the difference is within x% of the period, activate the "close" indicator
// Assumes two pulse rates are slow (in the Hz or 10s of Hz range), and are varying gradually
// compiled for the T3.1 ARM and the Arduino UNO with IDE 1.6.5
// David Smith, Nov 2015

// no included libraries

int led_pin = 13;  // built-in LED
int in1 = 1;       // input of 1 pulse stream
int in2 = 2;       // input of 2nd pulse stream
bool p1_state = 0;
bool p2_state = 0;
bool p1_state_prev = 99;
bool p2_state_prev = 99;
int t_p1 = 0;
int t_p1_prev = 0;
int t_p2 = 0;
int t_p2_prev = 0;
float duration1 = 0;
float duration2 = 0;
bool close_flag = 0;
float p1;
float p2;
bool first_flag1 = 1;
bool first_flag2 = 1;
float close_value = 0.05; // rate difference must be < x%

void setup()
{
  pinMode(led_pin, OUTPUT);
  pinMode(in1, INPUT_PULLUP);
  pinMode(in2, INPUT_PULLUP);
  digitalWrite(led_pin, 0); // start off, not yet "close"; ON when close
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {} // wait for port to activate on Teensy and Due
  Serial.println ("\nstart...\n");
}

void loop() {
  static int i = 0;
  // p1_state = digitalRead(in1);
  // p2_state = digitalRead(in2);
  p1 = sin(2 * 0.0100 * float(i));              // simulate input rate 1
  p2 = sin(2 * 0.0106 * float(i));   // simulate input rate 2 0.0103 close,  0.0107 > 5%
  p1_state = p1 > 0;
  p2_state = p2 > 0;

  //Serial.print(p1_state); Serial.print("  "); Serial.println(p2_state); // Serial.print (" 1 rise "); // temp

  if (p1_state > p1_state_prev)   // it's transitioned high
  {

    t_p1 = millis();
    duration1 = float(t_p1 - t_p1_prev);
    t_p1_prev = t_p1;
    p1_state_prev = p1_state;  // store state


    compareDurations(close_value);
    first_flag1 = 0;
  }
  else  if (p1_state < p1_state_prev)   // it's transitioned low
  {
    p1_state_prev = p1_state;
  }


  if (p2_state > p2_state_prev)   // it's transitioned high
  {
    t_p2 = millis();
    duration2 = float(t_p2 - t_p2_prev);
    t_p2_prev = t_p2;
    p2_state_prev = p2_state; // store state
    // Serial.print (" 2 rise ");  // temp
    first_flag2 = 0;
  }
  else  if (p2_state < p2_state_prev)   // it's transitioned low
  {
    p2_state_prev = p2_state;
  }

  // now you know the duration of each most recent cycle of inputs
  // test if they're similar
  // I put the comparison (arbitrarily) in the P1 test

  i = (i + 1) ;//* (i < 10000); // 0 - 999
  delay(1);
}

// --------------------------------------------------------------
void compareDurations(float close_val) {
  float delta_dur;
  if (first_flag1 == 0 && first_flag2 == 0) {
    // compute the difference in durations; normalize to compare by percent. flag is true fractional difference is < %
    delta_dur = abs(duration2 - duration1);
    close_flag = ( delta_dur / (duration1 + .001)) <= close_val;  // evaluates to 1 or 0

    Serial.print (int(duration1));
    Serial.print("  ");
    Serial.print (int(duration2));
    Serial.print("  ");
    Serial.print(delta_dur / duration1 * 100.0);
    Serial.print("%  ");

    if (close_flag) {
      digitalWrite(led_pin, 1);   // ON: indicator when rates are about the same
      Serial.println(" close! ");
    }
    else {
      digitalWrite(led_pin, 0);   // OFF
      Serial.println(" not.   ----------------");
    }
  }
}






















