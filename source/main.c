#include "kernel_utils.h"

int _main(struct thread *td) {
  initKernel();
  initLibc();
  initPthread();

  uint64_t fw_version = get_fw_version();
  jailbreak(fw_version);

  initSysUtil();

  char hfile[256];
  SceUserServiceLoginUserIdList userIdList = getUserIDList();

  for (int i = 0; i < SCE_USER_SERVICE_MAX_LOGIN_USERS; i++) {
    if (userIdList.userId[i] != -1 && userIdList.userId[i] != 0) {
      sprintf(hfile, "/user/home/%x/webbrowser/endhistory.txt", userIdList.userId[i]);
      if (!file_exists(hfile) && !dir_exists(hfile)) {
        mkdir(hfile, 0777);
        printf_notification("History blocker enabled for: %s\n\nRun payload again to disable", getUserName(userIdList.userId[i]));
      } else {
        if (dir_exists(hfile)) {
          rmdir(hfile);
          printf_notification("History blocker disabled for: %s\n\nRun payload again to enable", getUserName(userIdList.userId[i]));
        } else if (file_exists(hfile)) {
          unlink(hfile);
          mkdir(hfile, 0777);
          printf_notification("History blocker enabled for: %s\n\nRun payload again to disable", getUserName(userIdList.userId[i]));
        }
      }
    }
  }

  return 0;
}
