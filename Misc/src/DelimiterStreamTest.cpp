// Copyright  2007 Dieter Lucking
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include <iostream>
#include <iomanip>
#include <sstream>
#include "delimiter_stream.h"

using Poco::CsvInputStream;
using Poco::skip;
using std::cout;
using std::setw;
using std::endl;

int main()
{
	std::string bank_of_canada_exchange_rates(
    "# The daily noon exchange rates for major foreign currencies are published every business day at about 12:30 \r\n"
    "# p.m. EST. They are obtained from market or official sources around noon, and show the rates for the\r\n"
    "# various currencies in Canadian dollars converted from US dollars. The rates are nominal quotations -\r\n"
    "# neither buying nor selling rates - and are intended for statistical or analytical purposes. Rates\r\n"
    "# available from financial institutions will differ.\r\n"
    "#\r\n"
    "Date (<m>/<d>/<year>),11/05/2007,11/06/2007,11/07/2007,11/08/2007,11/09/2007,11/12/2007,11/13/2007\r\n"
    "Closing Can/US Exchange Rate,0.9330,0.9215,0.9281,0.9360,0.9428,N/A,N/A\r\n"
    "\"U.S. Dollar, Noon\",0.9339,0.9245,0.9170,0.9306,0.9391,NA,0.9643\r\n"
    "Argentina Peso (Floating Rate),0.2950,0.2922,0.2896,0.2951,0.2971,NA,0.3050\r\n"
    "Australian Dollar,0.8588,0.8554,0.8592,0.8622,0.8586,NA,0.8576\r\n"
    "Bahamian Dollar,0.9339,0.9245,0.9170,0.9306,0.9391,NA,0.9643\r\n"
    "Brazilian Real,0.5322,0.5341,0.5296,0.5360,0.5359,NA,0.5450\r\n"
    "Chilean Peso,0.001864,0.001839,0.001823,0.001847,0.001856,NA,0.001903\n"
    "Chinese Renminbi,0.1253,0.1240,0.1232,0.1254,0.1267,NA,0.1297\r\n"
    "Colombian Peso,0.000464,0.000459,0.000454,0.000458,0.000458,NA,0.000471\r\n"
    "Croatian Kuna,0.1840,0.1833,0.1834,0.1863,0.1879,NA,0.1915\r\n"
    "Czech Republic Koruna,0.05015,0.04998,0.04999,0.05091,0.05179,NA,0.05292\r\n"
    "Danish Krone,0.1813,0.1805,0.1804,0.1834,0.1847,NA,0.1889\r\n"
    "East Caribbean Dollar,0.3498,0.3463,0.3434,0.3512,0.3544,NA,0.3639\r\n"
    "European Euro,1.3512,1.3456,1.3448,1.3671,1.3771,NA,1.4078\r\n"
    "Fiji Dollar,0.6097,0.6034,0.6042,0.6120,0.6189,NA,0.6210\r\n"
    "CFA Franc (African Financial Community),0.002060,0.002051,0.002050,0.002084,0.002099,NA,0.002146\r\n"
    "CFP Franc (Pacific Financial Community),0.01132,0.01128,0.01127,0.01146,0.01154,NA,0.01180\r\n"
    "Ghanaian Cedi (new),0.9831,0.9757,0.9444,0.9577,0.9657,NA,1.0097\r\n"
    "Guatemalan Quetzal,0.1216,0.1206,0.1202,0.1217,0.1231,NA,0.1266\r\n"
    "Honduran Lempira,0.04943,0.04893,0.04853,0.04925,0.04970,NA,0.05103\r\n"
    "Hong Kong Dollar,0.120286,0.119087,0.118092,0.119828,0.120677,NA,0.123831\r\n"
    "Hungarian Forint,0.005348,0.005330,0.005309,0.005394,0.005424,NA,0.005541\r\n"
    "Icelandic Krona,0.01576,0.01575,0.01558,0.01563,0.01563,NA,0.01599\r\n"
    "Indian Rupee,0.02383,0.02356,0.02341,0.02378,0.02401,NA,0.02458\r\n"
    "Indonesian Rupiah,0.000102,0.000101,0.000101,0.000102,0.000103,NA,0.000105\r\n"
    "Israeli New Shekel,0.2357,0.2348,0.2336,0.2367,0.2388,NA,0.2441\r\n"
    "Jamaican Dollar,0.01313,0.01299,0.01289,0.01307,0.01319,NA,0.01354\r\n"
    "Japanese Yen,0.008154,0.008072,0.008095,0.008261,0.008467,NA,0.008741\r\n"
    "Malaysian Ringgit,0.2789,0.2770,0.2753,0.2794,0.2830,NA,0.2879\r\n"
    "Mexican Peso,0.08703,0.08638,0.08541,0.08603,0.08629,NA,0.08853\r\n"
    "Moroccan dirham,0.1194,0.1187,0.1184,0.1203,0.1214,NA,0.1240\r\n"
    "Myanmar (Burma) Kyat,0.1728,0.1714,0.1709,0.1735,0.1754,NA,0.1796\r\n"
    "Neth. Antilles Guilder,0.5247,0.5194,0.5152,0.5228,0.5276,NA,0.5417\r\n"
    "New Zealand Dollar,0.7162,0.7173,0.7187,0.7224,0.7201,NA,0.7273\r\n"
    "Norwegian Krone,0.1736,0.1728,0.1733,0.1768,0.1765,NA,0.1773\r\n"
    "Pakistan rupee,0.01533,0.01518,0.01506,0.01526,0.01540,NA,0.01578\r\n"
    "Panamanian Balboa,0.9339,0.9245,0.9170,0.9306,0.9391,NA,0.9643\r\n"
    "Peruvian New Sol,0.3112,0.3077,0.3053,0.3101,0.3137,NA,0.3223\r\n"
    "Philippine Peso,0.02137,0.02116,0.02125,0.02149,0.02199,NA,0.02252\r\n"
    "Polish Zloty,0.3709,0.3702,0.3696,0.3767,0.3785,NA,0.3866\r\n"
    "Romanian New Leu,0.4017,0.3989,0.3963,0.4021,0.4036,NA,0.4071\r\n"
    "Russian Rouble,0.03787,0.03761,0.03747,0.03805,0.03837,NA,0.03928\r\n"
    "Serbian Dinar,0.01753,0.01734,0.01713,0.01749,0.01760,NA,0.01808\r\n"
    "Singapore Dollar,0.6440,0.6389,0.6374,0.6468,0.6511,NA,0.6661\r\n"
    "Slovak Koruna,0.04060,0.04056,0.04055,0.04127,0.04176,NA,0.04279\r\n"
    "South African Rand,0.1419,0.1420,0.1422,0.1434,0.1415,NA,0.1434\r\n"
    "South Korean Won,0.001029,0.001018,0.001012,0.001025,0.001036,NA,0.001050\r\n"
    "Sri Lanka Rupee,0.00847,0.00841,0.00831,0.00844,0.00850,NA,0.00873\r\n"
    "Swedish Krona,0.1457,0.1455,0.1455,0.1477,0.1484,NA,0.1518\r\n"
    "Swiss Franc,0.8095,0.8071,0.8088,0.8260,0.8361,NA,0.8568\r\n"
    "Taiwanese New Dollar,0.02883,0.02854,0.02835,0.02880,0.02910,NA,0.02985\r\n"
    "Thai Baht,0.02950,0.02944,0.02898,0.02947,0.02967,NA,0.03054\r\n"
    "Trinidad & Tobago Dollar,0.1489,0.1474,0.1463,0.1484,0.1498,NA,0.1538\r\n"
    "Tunisian Dinar,0.7559,0.7507,0.7498,0.7603,0.7672,NA,0.7873\r\n"
    "New Turkish Lira,0.7871,0.7871,0.7807,0.7848,0.7835,NA,0.8078\r\n"
    "UAE Dirham,0.2544,0.2517,0.2497,0.2534,0.2558,NA,0.2627\r\n"
    "U.K. Pound Sterling,1.9423,1.9283,1.9296,1.9640,1.9638,NA,1.9988\r\n"
    "Venezuelan Bolivar,0.000435,0.000431,0.000427,0.000433,0.000437,NA,0.000449\r\n"
    "Vietnamese Dong,0.000058,0.000058,0.000057,0.000058,0.000058,NA,0.000060\r\n");

    std::stringstream stream(bank_of_canada_exchange_rates);
    CsvInputStream csv(stream);
    while(csv.skip_line('#'));
    std::string d[7];
    csv >> skip >> d[0] >> d[1] >> d[2] >> d[3] >> d[4] >> d[5] >> d[6];
    cout << d[0] << ", " << d[1] << ", " << d[2] << ", " << d[3] << ", "
	     << d[4] << ", " << d[5] << ", " << d[6] << std::endl;
	std::string description;
	std::string r;
	while(csv) {
		csv >> description;
    	cout << setw(40) << description;
		for(int i = 0; i < 7; ++i) {
			csv >> r;
			cout <<  ' '  << setw(8) << r;
		}
		cout << endl;
	}
}
	
