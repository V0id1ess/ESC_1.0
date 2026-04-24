class PI {
    public:
        PI(double kpGain, double kiGain)
            :kp(kpGain), ki(kiGain), integral(0.0) {}

        double compute(double setpoint, double measured_value, double dt) {
            double error = setpoint - measured_value;
            integral += error * dt;

            return kp * error + ki * integral;
        }
    
    private:
        double kp;
        double ki;
        double integral;
        double prev_time;
};