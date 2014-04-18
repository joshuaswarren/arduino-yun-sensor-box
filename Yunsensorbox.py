from subprocess import Popen, PIPE, STDOUT
import json

def getYun():
	cmd = '/usr/local/bin/php /usr/bin/sd-agent/plugins/yunsensorbox.php'
        p = Popen(cmd, shell=True, stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)
        output = p.stdout.read()
        o = json.loads(output)
        return_data = {}
        for index,value in enumerate(o):
                return_data[value] = o[value]
        return return_data

class Yunsensorbox:
    def __init__(self, config, logger, raw_config):
        self.config = config
        self.logger = logger
        self.raw_config = raw_config

    def run(self):
        return_data = getYun()
        return return_data