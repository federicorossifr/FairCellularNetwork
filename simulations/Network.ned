network Network
{
    parameters:
        int n;
    submodules:
        antenna: Antenna;
        user[n]: User;
    connections:
        for i=0..n-1 {
            antenna.out++ --> user[i].in;
            user[i].out --> antenna.in++;
        }
}
