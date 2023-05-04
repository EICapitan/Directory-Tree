# AT PROJECT Limited 2023
# by ElCapitan
pkgname=ds
_gitname="Directory-diSplay"
pkgver=v1.2.7
pkgrel=1
arch=('x86_64')
license=('GPL3')
source=("git+https://github.com/at-elcapitan/Directory-diSplay.git")
sha256sums=('SKIP')

build() {
    cd "$_gitname"
    cmake .
    make
}

package() {
    cd "$_gitname"
    make DESTDIR="$pkgdir/" install
    mkdir -p "$pkgdir/usr/share/man/man1"
    install -g 0 -o 0 -m 0644 ds.1 "$pkgdir/usr/share/man/man1/"
}
