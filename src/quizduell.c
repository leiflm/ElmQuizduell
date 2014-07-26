#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>
#include "Quizduell_Config.h"
#include "Quizduell_View.h"
#include "Quizduell_Connection.h"
#include "Quizduell_Controller.h"

// Account dependent
#define DATA_DIR "/tmp/"

Quizduell_Config qd_config;

// Config for Germany
Quizduell_Country_Specific_Config de_qcsc = {
    .encrypted_hmac_key = "32O6H/XmXyboLYJ3S+bC5hCrQuSldN7fur9MbU5g33A=",
    .encrypted_base_url = "rt6cSQ+b0gQcNs+Ka/XRtR7MMRvUnkQpmkSlYZLe3Bk=",
    .user_agent = "Quizduell A 1.3.2",
    .pem_file = ELM_QUIZDUELL_CERTS_DIR"qkgermany.pem"
};

// Config for Italy
Quizduell_Country_Specific_Config it_qcsc = {
    .encrypted_hmac_key = "P/bacQtJsv1nKoMPAacI7BCrQuSldN7fur9MbU5g33A=",
    .encrypted_base_url = "9ddNnke+FSDNh/v5AnM0BX4gdk0WOmIFtkwkEjv+NmA=",
    .user_agent = "Quizduelo A 1.3.2", //possibly missspelled, check APK!
    .pem_file = NULL
};

int main(int argc, char *argv[])
{
    eina_init();
    qd_config_init(&de_qcsc);
    qd_view_init(argc, argv);
    qd_con_init();
    qd_ctrl_init();

    qd_ctrl_run();

    qd_ctrl_shutdown();
    qd_view_shutdown();
    qd_con_shutdown();
    qd_config_shutdown();
    eina_shutdown();

    return EXIT_SUCCESS;
}