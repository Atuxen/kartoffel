class WifiHelper {
public:
    WifiHelper();
    bool setupWifi();          // ← was void, now returns true on success
private:
    /* keep your credential struct, etc. */
};
