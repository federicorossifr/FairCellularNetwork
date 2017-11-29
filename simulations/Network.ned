network Network
{
    parameters:
        int n;
        @display("bgb=415,377");
    submodules:
        antenna: Antenna;
        user[n]: User {
            @display("p=36,163");
        }
    connections:
        for i=0..n-1 {
            antenna.out[i] --> user[i].in;
            user[i].out --> antenna.in[i];
        }
}
