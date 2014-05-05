#include <stdio.h>
#include <stdlib.h>
#include "Quizduell_Config.h"
#include "Quizduell_Crypto.h"

// Account dependent
#define USER_AGENT "Quizduell A 1.3.2"

Quizduell_Config qd_config;

// Config for Germany
Quizduell_Country_Specific_Config de_qcsc = {
    .encrypted_hmac_key = "32O6H/XmXyboLYJ3S+bC5hCrQuSldN7fur9MbU5g33A=",
    .encrypted_base_url = "rt6cSQ+b0gQcNs+Ka/XRtR7MMRvUnkQpmkSlYZLe3Bk="
};

// Config for Italy
Quizduell_Country_Specific_Config it_qcsc = {
    .encrypted_hmac_key = "P/bacQtJsv1nKoMPAacI7BCrQuSldN7fur9MbU5g33A=",
    .encrypted_base_url = "9ddNnke+FSDNh/v5AnM0BX4gdk0WOmIFtkwkEjv+NmA="
};

int main(int arc, char *argv[])
{
    quizduell_config_init(&de_qcsc);
    quizduell_config_free();
    
    return EXIT_SUCCESS;
}