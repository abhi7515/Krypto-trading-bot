#ifndef K_KM_H_
#define K_KM_H_

namespace K {
  static const char kB64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                     "abcdefghijklmnopqrstuvwxyz"
                                     "0123456789+/";
  static int argPort = 3000,
             argColors = 0,
             argDebug = 0,
             argDebugEvents = 0,
             argDebugOrders = 0,
             argDebugQuotes = 0,
             argHeadless = 0,
             argNaked = 0,
             argAutobot = 0;
  extern int argFree;
  static string argTitle = "K.sh",
                argExchange = "NULL",
                argUser = "NULL",
                argPass = "NULL",
                argMatryoshka = "https://www.example.com/",
                argCurrency = "NULL",
                argApikey = "NULL",
                argSecret = "NULL",
                argUsername = "NULL",
                argPassphrase = "NULL",
                argHttp = "NULL",
                argWss = "NULL",
                argDatabase = "",
                argWhitelist = "";
  static double argEwmaShort = 0,
                argEwmaMedium = 0,
                argEwmaLong = 0;
  enum class mExchange: unsigned int { Null, HitBtc, OkCoin, Coinbase, Bitfinex, Korbit, Poloniex };
  enum class mGatewayType: unsigned int { MarketData, OrderEntry };
  enum class mTimeInForce: unsigned int { IOC, FOK, GTC };
  enum class mConnectivity: unsigned int { Disconnected, Connected };
  enum class mOrderType: unsigned int { Limit, Market };
  enum class mSide: unsigned int { Bid, Ask, Unknown };
  enum class mORS: unsigned int { New, Working, Complete, Cancelled };
  enum class mPingAt: unsigned int { BothSides, BidSide, AskSide, DepletedSide, DepletedBidSide, DepletedAskSide, StopPings };
  enum class mPongAt: unsigned int { ShortPingFair, LongPingFair, ShortPingAggressive, LongPingAggressive };
  enum class mQuotingMode: unsigned int { Top, Mid, Join, InverseJoin, InverseTop, HamelinRat, Depth };
  enum class mQuotingSafety: unsigned int { Off, PingPong, Boomerang, AK47 };
  enum class mQuoteState: unsigned int { Live, Disconnected, DisabledQuotes, MissingData, UnknownHeld, TBPHeld, MaxTradesSeconds, WaitingPing, DepletedFunds, Crossed, UpTrendHeld, DownTrendHeld };
  enum class mFairValueModel: unsigned int { BBO, wBBO };
  enum class mAutoPositionMode: unsigned int { Manual, EWMA_LS, EWMA_LMS };
  enum class mAPR: unsigned int { Off, Size, SizeWidth };
  enum class mSOP: unsigned int { Off, Trades, Size, TradesSize };
  enum class mSTDEV: unsigned int { Off, OnFV, OnFVAPROff, OnTops, OnTopsAPROff, OnTop, OnTopAPROff };
  enum class uiBIT: unsigned char { MSG = '-', SNAP = '=' };
  enum class uiTXT: unsigned char {
    FairValue = 'a', Quote = 'b', ActiveSubscription = 'c', ActiveState = 'd', MarketData = 'e',
    QuotingParametersChange = 'f', SafetySettings = 'g', Product = 'h', OrderStatusReports = 'i',
    ProductAdvertisement = 'j', ApplicationState = 'k', Notepad = 'l', ToggleSettings = 'm',
    Position = 'n', ExchangeConnectivity = 'o', SubmitNewOrder = 'p', CancelOrder = 'q',
    MarketTrade = 'r', Trades = 's', ExternalValuation = 't', QuoteStatus = 'u',
    TargetBasePosition = 'v', TradeSafetyValue = 'w', CancelAllOrders = 'x',
    CleanAllClosedTrades = 'y', CleanAllTrades = 'z', CleanTrade = 'A', TradesChart = 'B',
    WalletChart = 'C', EWMAChart = 'D'
  };
  static char RBLACK[] = "\033[0;30m", RRED[]    = "\033[0;31m", RGREEN[] = "\033[0;32m", RYELLOW[] = "\033[0;33m",
              RBLUE[]  = "\033[0;34m", RPURPLE[] = "\033[0;35m", RCYAN[]  = "\033[0;36m", RWHITE[]  = "\033[0;37m",
              BBLACK[] = "\033[1;30m", BRED[]    = "\033[1;31m", BGREEN[] = "\033[1;32m", BYELLOW[] = "\033[1;33m",
              BBLUE[]  = "\033[1;34m", BPURPLE[] = "\033[1;35m", BCYAN[]  = "\033[1;36m", BWHITE[]  = "\033[1;37m";
  struct mPair {
    string base,
           quote;
    mPair():
      base(""), quote("")
    {};
    mPair(string b, string q):
      base(b), quote(q)
    {};
  };
  struct mWallet {
    double amount,
           held;
    string currency;
    mWallet():
      amount(0), held(0), currency("")
    {};
    mWallet(double a, double h, string c):
      amount(a), held(h), currency(c)
    {};
  };
  static void to_json(json& j, const mWallet& k) {
    j = {
      {"amount", k.amount},
      {"held", k.held},
      {"currency", k.currency}
    };
  };
  struct mProfit {
           double baseValue,
                  quoteValue;
    unsigned long time;
    mProfit(double b, double q, unsigned long t):
      baseValue(b), quoteValue(q), time(t)
    {};
  };
  static void to_json(json& j, const mProfit& k) {
    j = {
      {"baseValue", k.baseValue},
      {"quoteValue", k.quoteValue},
      {"time", k.time}
    };
  };
  struct mSafety {
    double buy,
           sell,
           combined,
           buyPing,
           sellPong;
    mSafety():
      buy(0), sell(0), combined(0), buyPing(-1), sellPong(-1)
    {};
    mSafety(double b, double s, double c, double bP, double sP):
      buy(b), sell(s), combined(c), buyPing(bP), sellPong(sP)
    {};
  };
  static void to_json(json& j, const mSafety& k) {
    j = {
      {"buy", k.buy},
      {"sell", k.sell},
      {"combined", k.combined},
      {"buyPing", k.buyPing},
      {"sellPong", k.sellPong}
    };
  };
  struct mPosition {
       double baseAmount,
              quoteAmount,
              baseHeldAmount,
              quoteHeldAmount,
              value,
              quoteValue,
              profitBase,
              profitQuote;
        mPair pair;
    mExchange exchange;
    mPosition():
      baseAmount(0), quoteAmount(0), baseHeldAmount(0), quoteHeldAmount(0), value(0), quoteValue(0), profitBase(0), profitQuote(0), pair(mPair()), exchange((mExchange)0)
    {};
    mPosition(double bA, double qA, double bH, double qH, double bV, double qV, double bP, double qP, mPair p, mExchange e):
      baseAmount(bA), quoteAmount(qA), baseHeldAmount(bH), quoteHeldAmount(qH), value(bV), quoteValue(qV), profitBase(bP), profitQuote(qP), pair(p), exchange(e)
    {};
  };
  static void to_json(json& j, const mPosition& k) {
    j = {
      {"baseAmount", k.baseAmount},
      {"quoteAmount", k.quoteAmount},
      {"baseHeldAmount", k.baseHeldAmount},
      {"quoteHeldAmount", k.quoteHeldAmount},
      {"value", k.value},
      {"quoteValue", k.quoteValue},
      {"profitBase", k.profitBase},
      {"profitQuote", k.profitQuote},
      {"pair", {{"base", k.pair.base}, {"quote", k.pair.quote}}},
      {"exchange", (int)k.exchange}
    };
  };
  struct mTrade {
           string tradeId;
        mExchange exchange;
            mSide side;
            mPair pair;
           double price,
                  quantity,
                  value,
                  Kqty,
                  Kvalue,
                  Kprice,
                  Kdiff,
                  feeCharged;
    unsigned long time,
                  Ktime;
             bool loadedFromDB;

    mTrade():
      tradeId(""), exchange((mExchange)0), pair(mPair()), price(0), quantity(0), side((mSide)0), time(0), value(0), Ktime(0), Kqty(0), Kprice(0), Kvalue(0), Kdiff(0), feeCharged(0), loadedFromDB(false)
    {};
    mTrade(double p, double q, unsigned long t):
      tradeId(""), exchange((mExchange)0), pair(mPair()), price(p), quantity(q), side((mSide)0), time(t), value(0), Ktime(0), Kqty(0), Kprice(0), Kvalue(0), Kdiff(0), feeCharged(0), loadedFromDB(false)
    {};
    mTrade(double p, double q, mSide s):
      tradeId(""), exchange((mExchange)0), pair(mPair()), price(p), quantity(q), side(s), time(0), value(0), Ktime(0), Kqty(0), Kprice(0), Kvalue(0), Kdiff(0), feeCharged(0), loadedFromDB(false)
    {};
    mTrade(string i, mExchange e, mPair P, double p, double q, mSide S, unsigned long t, double v, unsigned long Kt, double Kq, double Kp, double Kv, double Kd, double f, bool l):
      tradeId(i), exchange(e), pair(P), price(p), quantity(q), side(S), time(t), value(v), Ktime(Kt), Kqty(Kq), Kprice(Kp), Kvalue(Kv), Kdiff(Kd), feeCharged(f), loadedFromDB(l)
    {};
  };
  static void to_json(json& j, const mTrade& k) {
    j = {
      {"tradeId", k.tradeId},
      {"time", k.time},
      {"exchange", (int)k.exchange},
      {"pair", {{"base", k.pair.base}, {"quote", k.pair.quote}}},
      {"price", k.price},
      {"quantity", k.quantity},
      {"side", (int)k.side},
      {"value", k.value},
      {"Ktime", k.Ktime},
      {"Kqty", k.Kqty},
      {"Kprice", k.Kprice},
      {"Kvalue", k.Kvalue},
      {"Kdiff", k.Kdiff},
      {"feeCharged", k.feeCharged},
      {"loadedFromDB", k.loadedFromDB},
    };
  };
  struct mOrder {
           string orderId,
                  exchangeId;
        mExchange exchange;
            mPair pair;
            mSide side;
           double price,
                  quantity,
                  lastQuantity;
       mOrderType type;
     mTimeInForce timeInForce;
             mORS orderStatus;
             bool isPong,
                  preferPostOnly;
    unsigned long time,
                  computationalLatency;
    mOrder():
      orderId(""), exchangeId(""), exchange((mExchange)0), pair(mPair()), side((mSide)0), quantity(0), type((mOrderType)0), isPong(false), price(0), timeInForce((mTimeInForce)0), orderStatus((mORS)0), preferPostOnly(false), lastQuantity(0), time(0), computationalLatency(0)
    {};
    mOrder(string o, mORS s):
      orderId(o), exchangeId(""), exchange((mExchange)0), pair(mPair()), side((mSide)0), quantity(0), type((mOrderType)0), isPong(false), price(0), timeInForce((mTimeInForce)0), orderStatus(s), preferPostOnly(false), lastQuantity(0), time(0), computationalLatency(0)
    {};
    mOrder(string o, string e, mORS s, double p, double q, double Q):
      orderId(o), exchangeId(e), exchange((mExchange)0), pair(mPair()), side((mSide)0), quantity(q), type((mOrderType)0), isPong(false), price(p), timeInForce((mTimeInForce)0), orderStatus(s), preferPostOnly(false), lastQuantity(Q), time(0), computationalLatency(0)
    {};
    mOrder(string o, mExchange e, mPair P, mSide S, double q, mOrderType t, bool i, double p, mTimeInForce F, mORS s, bool O):
      orderId(o), exchangeId(""), exchange(e), pair(P), side(S), quantity(q), type(t), isPong(i), price(p), timeInForce(F), orderStatus(s), preferPostOnly(O), lastQuantity(0), time(0), computationalLatency(0)
    {};
  };
  static void to_json(json& j, const mOrder& k) {
    j = {
      {"orderId", k.orderId},
      {"exchangeId", k.exchangeId},
      {"exchange", (int)k.exchange},
      {"pair", {{"base", k.pair.base}, {"quote", k.pair.quote}}},
      {"side", (int)k.side},
      {"quantity", k.quantity},
      {"type", (int)k.type},
      {"isPong", k.isPong},
      {"price", k.price},
      {"timeInForce", (int)k.timeInForce},
      {"orderStatus", (int)k.orderStatus},
      {"preferPostOnly", k.preferPostOnly},
      {"lastQuantity", k.lastQuantity},
      {"time", k.time},
      {"computationalLatency", k.computationalLatency}
    };
  };
  struct mLevel {
    double price,
           size;
    mLevel():
      price(0), size(0)
    {};
    mLevel(double p, double s):
      price(p), size(s)
    {};
  };
  struct mLevels {
    vector<mLevel> bids,
                   asks;
    mLevels():
      bids({}), asks({})
    {};
    mLevels(vector<mLevel> b, vector<mLevel> a):
      bids(b), asks(a)
    {};
  };
  static void to_json(json& j, const mLevels& k) {
    json b, a;
    for (vector<mLevel>::const_iterator it = k.bids.begin(); it != k.bids.end(); ++it)
      b.push_back({{"price", it->price}, {"size", it->size}});
    for (vector<mLevel>::const_iterator it = k.asks.begin(); it != k.asks.end(); ++it)
      a.push_back({{"price", it->price}, {"size", it->size}});
    j = {{"bids", b}, {"asks", a}};
  };
  struct mQuote {
    mLevel bid,
           ask;
      bool isBidPong,
           isAskPong;
    mQuote():
      bid(mLevel()), ask(mLevel()), isBidPong(false), isAskPong(false)
    {};
    mQuote(mLevel b, mLevel a):
      bid(b), ask(a), isBidPong(false), isAskPong(false)
    {};
    mQuote(mLevel b, mLevel a, bool bP, bool aP):
      bid(b), ask(a), isBidPong(bP), isAskPong(aP)
    {};
  };
  static void to_json(json& j, const mQuote& k) {
    j = {
      {"bid", {
        {"price", k.bid.price},
        {"size", k.bid.size}
      }},
      {"ask", {
        {"price", k.ask.price},
        {"size", k.ask.size}
      }}
    };
  };
  struct mQuoteStatus {
     mQuoteState bidStatus,
                 askStatus;
    unsigned int quotesInMemoryNew,
                 quotesInMemoryWorking,
                 quotesInMemoryDone;
    mQuoteStatus():
      bidStatus((mQuoteState)0), askStatus((mQuoteState)0), quotesInMemoryNew(0), quotesInMemoryWorking(0), quotesInMemoryDone(0)
    {};
    mQuoteStatus(mQuoteState b, mQuoteState a, unsigned int n, unsigned int w, unsigned int d):
      bidStatus(b), askStatus(a), quotesInMemoryNew(n), quotesInMemoryWorking(w), quotesInMemoryDone(d)
    {};
  };
  static void to_json(json& j, const mQuoteStatus& k) {
    j = {
      {"bidStatus", (int)k.bidStatus},
      {"askStatus", (int)k.askStatus},
      {"quotesInMemoryNew", k.quotesInMemoryNew},
      {"quotesInMemoryWorking", k.quotesInMemoryWorking},
      {"quotesInMemoryDone", k.quotesInMemoryDone}
    };
  };
  static function<void(int)> *evExit;
  extern bool wInit;
  extern WINDOW *wBorder,
                *wLog;
  extern mutex wsMutex;
  static mutex wMutex,
               ogMutex,
               pgMutex;
  static string uiPrtcl = "?";
  static map<string, mOrder> allOrders;
  class Gw {
    public:
      static Gw *E(mExchange e);
      string (*randId)() = 0;
      function<void(mOrder)>        evDataOrder;
      function<void(mTrade)>        evDataTrade;
      function<void(mWallet)>       evDataWallet;
      function<void(mLevels)>       evDataLevels;
      function<void(mConnectivity)> evConnectOrder,
                                    evConnectMarket;
      uWS::Hub                *hub = nullptr;
      uWS::Group<uWS::CLIENT> *gwGroup = nullptr;
      mExchange exchange = mExchange::Null;
       double makeFee = 0,  minTick = 0,
              takeFee = 0,  minSize = 0;
       string base    = "", quote   = "",
              name    = "", symbol  = "",
              apikey  = "", secret  = "",
              user    = "", pass    = "",
              ws      = "", http    = "";
      virtual void wallet() = 0,
                   levels() = 0,
                   send(string oI, mSide oS, double oP, double oQ, mOrderType oLM, mTimeInForce oTIF, bool oPO, unsigned long oT) = 0,
                   cancel(string oI, string oE, mSide oS, unsigned long oT) = 0,
                   cancelAll() = 0,
                   close() = 0;
  };
  class Klass {
    protected:
      Klass *config = nullptr,
            *events = nullptr,
            *memory = nullptr,
            *client = nullptr,
            *params = nullptr,
            *orders = nullptr,
            *market = nullptr,
            *wallet = nullptr,
            *engine = nullptr;
      virtual void load(int argc, char** argv) {};
      virtual void load() {};
      virtual void waitTime() {};
      virtual void waitData() {};
      virtual void waitUser() {};
      virtual void run() {};
    public:
      void cfLink(Klass *k) { config = k; };
      void evLink(Klass *k) { events = k; };
      void dbLink(Klass *k) { memory = k; };
      void uiLink(Klass *k) { client = k; };
      void qpLink(Klass *k) { params = k; };
      void ogLink(Klass *k) { orders = k; };
      void mgLink(Klass *k) { market = k; };
      void pgLink(Klass *k) { wallet = k; };
      void qeLink(Klass *k) { engine = k; };
      void main(int argc, char** argv) {
        load(argc, argv);
        run();
      };
      void wait() {
        load();
        waitTime();
        waitData();
        waitUser();
        run();
      };
  };
  class kLass: public Klass {
    public:
      void link(Klass *EV, Klass *DB, Klass *UI, Klass *QP, Klass *OG, Klass *MG, Klass *PG, Klass *QE, Klass *GW) {
        UI->evLink(EV); QP->evLink(EV); OG->evLink(EV); MG->evLink(EV); PG->evLink(EV); QE->evLink(EV); GW->evLink(EV);
        UI->dbLink(DB); QP->dbLink(DB); OG->dbLink(DB); MG->dbLink(DB); PG->dbLink(DB);
                        QP->uiLink(UI); OG->uiLink(UI); MG->uiLink(UI); PG->uiLink(UI); QE->uiLink(UI); GW->uiLink(UI);
                                        OG->qpLink(QP);                 PG->qpLink(QP); QE->qpLink(QP);
                                                                        PG->ogLink(OG); QE->ogLink(OG);
                                                                                        QE->mgLink(MG);
                                                                                        QE->pgLink(PG);
                                                                                                        GW->qeLink(QE);
      };
  };
}

#endif
