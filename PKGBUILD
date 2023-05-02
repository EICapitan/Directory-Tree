# AT PROJECT Limited 2023
# by ElCapitan
pkgname=ds
_gitname="Directory-diSplay-atdt-"
pkgver=v1.2.7
pkgrel=1
arch=('x86_64')
license=('GPL3')
source=("$pkgname-$pkgver.tar.gz::https://github.com/EICapitan/Directory-Tree/archive/refs/tags/atdt-$pkgver.tar.gz")
sha256sums=('SKIP')

build() {
    cd "$_gitname$pkgver"
    cmake .
    make
}

package() {
    cd "$_gitname$pkgver"
    make DESTDIR="$pkgdir/" install
    mkdir -p "$pkgdir/usr/share/man/man1"
    install -g 0 -o 0 -m 0644 ds.1 "/usr/share/man/man1/"
}
