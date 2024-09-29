let pkgs = import <nixpkgs> { };
in pkgs.mkShell {
  name = "Tesla Interrupter";
  buildInputs = with pkgs; [ platformio ccls ];
}
