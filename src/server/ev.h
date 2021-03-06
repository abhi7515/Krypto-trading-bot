#ifndef K_EV_H_
#define K_EV_H_

namespace K  {
  uWS::Hub hub(0, true);
  class EV: public Klass {
    protected:
      void load() {
        evExit = &happyEnding;
        signal(SIGINT, quit);
        signal(SIGUSR1, wtf);
        signal(SIGABRT, wtf);
        signal(SIGSEGV, wtf);
        uv_timer_init(hub.getLoop(), &tWallet);
        uv_timer_init(hub.getLoop(), &tCancel);
        uv_timer_init(hub.getLoop(), &tStart);
        uv_timer_init(hub.getLoop(), &tCalcs);
        uv_timer_init(hub.getLoop(), &tDelay);
        gw->hub = &hub;
        gw->gwGroup = hub.createGroup<uWS::CLIENT>();
        uiGroup = hub.createGroup<uWS::SERVER>(uWS::PERMESSAGE_DEFLATE);
      };
      void run() {
        if (FN::output("test -d .git || echo -n zip") == "zip")
          FN::logVer("", -1);
        else {
          FN::output("git fetch");
          string k = changelog();
          FN::logVer(k, count(k.begin(), k.end(), '\n'));
        }
      };
    public:
      uWS::Group<uWS::SERVER> *uiGroup = nullptr;
      uv_timer_t tCalcs,
                 tStart,
                 tDelay,
                 tWallet,
                 tCancel;
      function<void(mOrder)> ogOrder;
      function<void(mTrade)> ogTrade;
      function<void()>       mgLevels,
                             mgEwmaSMUProtection,
                             mgEwmaQuoteProtection,
                             mgTargetPosition,
                             pgTargetBasePosition,
                             uiQuotingParameters;
      void start() {
        hub.run();
        halt(eCode);
      };
      void stop(int code) {
        eCode = code;
        if (uv_loop_alive(hub.getLoop())) {
          uv_timer_stop(&tCancel);
          uv_timer_stop(&tWallet);
          uv_timer_stop(&tCalcs);
          uv_timer_stop(&tStart);
          uv_timer_stop(&tDelay);
          gw->close();
          gw->gwGroup->close();
          FN::log(string("GW ") + argExchange, "Attempting to cancel all open orders, please wait.");
          gw->cancelAll();
          FN::log(string("GW ") + argExchange, "cancell all open orders OK");
          uiGroup->close();
          FN::close(hub.getLoop());
          hub.getLoop()->destroy();
        }
        halt(code);
      };
      void listen() {
        if ((access("etc/sslcert/server.crt", F_OK) != -1) and (access("etc/sslcert/server.key", F_OK) != -1)
          and hub.listen(argPort, uS::TLS::createContext("etc/sslcert/server.crt", "etc/sslcert/server.key", ""), 0, uiGroup)
        ) uiPrtcl = "HTTPS";
        else if (hub.listen(argPort, nullptr, 0, uiGroup))
          uiPrtcl = "HTTP";
        else FN::logExit("IU", string("Use another UI port number, ")
          + to_string(argPort) + " seems already in use by:\n"
          + FN::output(string("netstat -anp 2>/dev/null | grep ") + to_string(argPort)),
          EXIT_SUCCESS);
        FN::logUI(uiPrtcl, argPort);
      }
    private:
      int eCode = EXIT_FAILURE;
      void halt(int code) {
        cout << FN::uiT() << "K exit code " << to_string(code) << "." << '\n';
        exit(code);
      };
      function<void(int)> happyEnding = [&](int code) {
        cout << FN::uiT();
        for(unsigned int i = 0; i < 21; ++i)
          cout << "THE END IS NEVER ";
        cout << "THE END" << '\n';
        halt(code);
      };
      static void quit(int sig) {
        FN::screen_quit();
        cout << '\n';
        json k = FN::wJet("https://api.icndb.com/jokes/random?escape=javascript&limitTo=[nerdy]");
        cout << FN::uiT() << "Excellent decision! "
          << ((k.is_null() || !k["/value/joke"_json_pointer].is_string())
            ? "let's plant a tree instead.." : k["/value/joke"_json_pointer].get<string>()
          ) << '\n';
        (*evExit)(EXIT_SUCCESS);
      };
      static void wtf(int sig) {
        FN::screen_quit();
        cout << FN::uiT() << RCYAN << "Errrror: Signal " << sig << " "  << strsignal(sig);
        if (latest()) {
          cout << " (Three-Headed Monkey found)." << '\n';
          report();
          this_thread::sleep_for(chrono::seconds(3));
        } else {
          cout << " (deprecated K version found)." << '\n';
          upgrade();
          this_thread::sleep_for(chrono::seconds(21));
        }
        (*evExit)(EXIT_FAILURE);
      };
      static bool latest() {
        return FN::output("test -d .git && git rev-parse @") == FN::output("test -d .git && git rev-parse @{u}");
      }
      static string changelog() {
        return FN::output("test -d .git && git --no-pager log --graph --oneline @..@{u}");
      }
      static void upgrade() {
        cout << '\n' << BYELLOW << "Hint!" << RYELLOW
          << '\n' << "please upgrade to the latest commit; the encountered error may be already fixed at:"
          << '\n' << changelog()
          << '\n' << "If you agree, consider to run \"make latest\" prior further executions."
          << '\n' << '\n';
      };
      static void report() {
        void *k[69];
        backtrace_symbols_fd(k, backtrace(k, 69), STDERR_FILENO);
        cout << '\n' << BRED << "Yikes!" << RRED
          << '\n' << "please copy and paste the error above into a new github issue (noworry for duplicates)."
          << '\n' << "If you agree, go to https://github.com/ctubio/Krypto-trading-bot/issues/new"
          << '\n' << '\n';
      };
  };
}

#endif
