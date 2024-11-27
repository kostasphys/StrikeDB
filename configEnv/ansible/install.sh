#! usr/bin/bash

set -e


#Make all the necessary boostraping.
ansible-playbook   --extra-vars  upassword=pass1234   bootstrap.yml


sed  -i 's/remote_user = root/remote_user = dbAdmin/g'  ansible.cfg


#Setup the environment
ansible-playbook  setup_env.yml



