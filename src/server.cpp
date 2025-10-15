// Single-header deps: https://github.com/yhirose/cpp-httplib (copy httplib.h here)
// #define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "encryptor.hpp"
#include "decryptor.hpp"

#include <string>
#include <vector>
#include <iostream>

int main()
{
    const char *host = "0.0.0.0";
    const int port = 9090;

    std::string aes_key = manage_key("AES_KEY");
    auto expanded = AES128KeyExpansion(aes_key);

    httplib::Server svr;
    svr.set_payload_max_length(100 * 1024 * 1024);

    auto handle = [expanded](const httplib::Request &req, httplib::Response &res, bool do_encrypt)
    {
        try
        {
            const auto &body = req.body;

            std::string out = do_encrypt ? encrypt(body, expanded) : decrypt(body, expanded);

            res.set_content(out, "application/octet-stream");
            res.status = 200;
            cout << "[INFO]: request handled\n";
        }
        catch (const std::exception &e)
        {
            res.status = 500;
            res.set_content(std::string("error: ") + e.what(), "text/plain");
        }
        catch (...)
        {
            res.status = 500;
            res.set_content("internal error", "text/plain");
        }
    };

    svr.Post("/encrypt", [handle](const httplib::Request &req, httplib::Response &res)
             { handle(req, res, true); });
    svr.Post("/decrypt", [handle](const httplib::Request &req, httplib::Response &res)
             { handle(req, res, false); });
    svr.Get("/health", [](const httplib::Request &, httplib::Response &res)
            { res.set_content("ok", "text/plain"); cout<< "[INFO]: health checked\n"; });
    svr.Get("/", [](const httplib::Request &, httplib::Response &res)
            { std::ifstream f("index.html", std::ios::binary); std::ostringstream ss; ss << f.rdbuf(); 
                res.set_content(ss.str(), "text/html; charset=utf-8"); });
    std::cout << "Listening on http://" << host << ":" << port << std::endl;
    svr.listen(host, port);
    return 0;
}
