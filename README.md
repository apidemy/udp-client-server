# Better Repository
  Asio comes in two variants: (non-Boost) Asio and Boost.Asio.
  There are better examples in Asio source repositoris to use UDP (TCP or etc.) sockets.
 ## Where do I get Asio release package?

  Asio is available for download from SourceForge, in a package named asio-X.Y.Z.tar.gz (or .tar.bz2 or .zip).

  Boost.Asio is included in the Boost distributions for version 1.35 and later. It is also available as a separate package on SourceForge, named         boost_asio_X_Y_Z.tar.gz. The latter is intended to be copied over the top of an existing Boost source code distribution.
 
 ## Where are the source code repositories?

  Asio is hosted on [GitHub.](http://github.com/chriskohlhoff/asio/)

  Boost.Asio is checked into [Boost’s GitHub repository.](http://github.com/boostorg/asio/)


# UDP Client Server
  You can use UDP(Server and Client) easily in your project, but meet the requirements :). There is an example in main.cpp of using these classes.

What this repo does:
  - A class for creating UDP socket as Server
  - A class for creating UDP socket as Client
  - Asynchronous UDP sockets
  
 ## Requirements:
  - Boost Asio
  - pthread
