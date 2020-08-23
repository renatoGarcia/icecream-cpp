{ pkgs ? import <nixpkgs> {} }:

let
  headerSrc = builtins.readFile ./CMakeLists.txt;

  versionNumbersRegex = ".*project[[:space:]]*\\(.*[[:space:]]VERSION[[:space:]]+([[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+).*\\).*";

  versionNumbers = builtins.head (builtins.match versionNumbersRegex headerSrc);

  devHash_ = builtins.match ".*DEV_HASH: ([[:xdigit:]]+).*" headerSrc;
  devHash = if devHash_ != null then builtins.head devHash_ else null;

  repo = builtins.fetchGit {
    url = https://github.com/renatoGarcia/icecream-cpp.git;
    ref = "master";
    rev = devHash;
  };

  version =
    versionNumbers +
    (
      if devHash != null
      then "+dev." + toString repo.revCount
      else "+local_repo"
    );

in pkgs.stdenv.mkDerivation {
  pname = "icecream-cpp";
  inherit version;

  src = ./.;

  nativeBuildInputs = [ pkgs.cmake ];

  meta = with pkgs.lib; {
    homepage = "https://github.com/renatoGarcia/icecream-cpp";
    description = "A little library to help with the print debugging on C++11 and forward";
    license = licenses.mit;
    maintainers = with maintainers; [ renatoGarcia ];
  };
}
