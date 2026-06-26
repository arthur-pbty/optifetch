Name:       optifetch
Version:    1.0.0
Release:    1%{?dist}
Summary:    Lightweight system info tool written in C
License:    MIT
URL:        https://github.com/arthur-pbty/optifetch
Source0:    %{url}/archive/v%{version}/%{name}-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make

%description
Optifetch is a fast, dependency-free system information tool written in C.

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
make DESTDIR=%{buildroot} install

%files
%{_bindir}/optifetch
%{_datadir}/optifetch/logos/
%config(noreplace) /etc/optifetch.conf