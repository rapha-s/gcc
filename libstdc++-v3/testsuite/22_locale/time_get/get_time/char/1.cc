// 2001-09-21 Benjamin Kosnik  <bkoz@redhat.com>

// Copyright (C) 2001, 2002, 2003 Free Software Foundation
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// 22.2.5.1.1 time_get members

#include <locale>
#include <sstream>
#include <testsuite_hooks.h>

void test01()
{
  using namespace std;
  bool test = true;
  typedef time_base::dateorder dateorder;
  typedef istreambuf_iterator<char> iterator_type;

  // basic construction and sanity checks.
  locale loc_c = locale::classic();
  locale loc_hk("en_HK");
  locale loc_fr("fr_FR@euro");
  locale loc_de("de_DE");
  VERIFY( loc_hk != loc_c );
  VERIFY( loc_hk != loc_fr );
  VERIFY( loc_hk != loc_de );
  VERIFY( loc_de != loc_fr );

  // cache the __timepunct facets, for quicker gdb inspection
  const __timepunct<char>& time_c = use_facet<__timepunct<char> >(loc_c); 
  const __timepunct<char>& time_de = use_facet<__timepunct<char> >(loc_de); 
  const __timepunct<char>& time_hk = use_facet<__timepunct<char> >(loc_hk); 
  const __timepunct<char>& time_fr = use_facet<__timepunct<char> >(loc_fr); 

  const string empty;

  // create an ostream-derived object, cache the time_get facet
  iterator_type end;
  istringstream iss;
  iss.imbue(loc_c);
  const time_get<char>& tim_get = use_facet<time_get<char> >(iss.getloc()); 

  const ios_base::iostate good = ios_base::goodbit;
  ios_base::iostate errorstate = good;

  // create "C" time objects
  const tm time_bday = { 0, 0, 12, 4, 3, 71 };
  const char* all = "%a %A %b %B %c %d %H %I %j %m %M %p %s %U "
                    "%w %W %x %X %y %Y %Z %%";
  const char* date = "%A, the second of %B";
  const char* date_ex = "%Ex";

  // 2
  // iter_type 
  // get_time(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const

  // sanity checks for "C" locale
  iss.str("12:00:00");
  iterator_type is_it01(iss);
  tm time01;
  errorstate = good;
  tim_get.get_time(is_it01, end, iss, errorstate, &time01);
  VERIFY( time01.tm_sec == time_bday.tm_sec );
  VERIFY( time01.tm_min == time_bday.tm_min );
  VERIFY( time01.tm_hour == time_bday.tm_hour );
  VERIFY( errorstate == ios_base::eofbit );

  iss.str("12:00:00 ");
  iterator_type is_it02(iss);
  tm time02;
  errorstate = good;
  tim_get.get_time(is_it02, end, iss, errorstate, &time02);
  VERIFY( time01.tm_sec == time_bday.tm_sec );
  VERIFY( time01.tm_min == time_bday.tm_min );
  VERIFY( time01.tm_hour == time_bday.tm_hour );
  VERIFY( errorstate == good );

  iss.str("12:61:00 ");
  iterator_type is_it03(iss);
  tm time03;
  errorstate = good;
  tim_get.get_time(is_it03, end, iss, errorstate, &time03);
  VERIFY( time01.tm_hour == time_bday.tm_hour );
  VERIFY( errorstate == ios_base::failbit );

  iss.str("12:a:00 ");
  iterator_type is_it04(iss);
  tm time04;
  errorstate = good;
  tim_get.get_time(is_it04, end, iss, errorstate, &time04);
  VERIFY( time01.tm_hour == time_bday.tm_hour );
  VERIFY( *is_it04 == 'a');
  VERIFY( errorstate == ios_base::failbit );

  // inspection of named locales, de_DE
  iss.imbue(loc_de);
  iss.str("12:00:00");
  iterator_type is_it10(iss);
  tm time10;
  errorstate = good;
  tim_get.get_time(is_it10, end, iss, errorstate, &time10);
  VERIFY( time10.tm_sec == time_bday.tm_sec );
  VERIFY( time10.tm_min == time_bday.tm_min );
  VERIFY( time10.tm_hour == time_bday.tm_hour );
  VERIFY( errorstate == ios_base::eofbit );
}

int main()
{
  test01();
  return 0;
}
