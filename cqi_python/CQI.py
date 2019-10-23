from ray.tune import trainable
# information about trainable.Trainable: https://ray.readthedocs.io/en/latest/tune-package-ref.html#ray.tune.Trainable


class CQITrainable(trainable.Trainable):
    def _setup(self, config):
        # TODO perform setup such as setting hyperparams, pass on to C
        pass

    def _train(self):
        # TODO -- actual training method, needs to be spec'd out more
        pass

    def _save(self, tmp_checkpoint_dir):
        # TODO wrapper, should call C code to save a compact format of decision tree with all internal information
        pass

    def _restore(self, checkpoint):
        # TODO wrapper, should call C code to load in a compact format of decision tree
        pass

    def _export_model(self, export_formats, export_dir):
        # TODO will save a final decision tree mapping, more compact, just with information necessary to traverse and
        #       values at end, no Q-values or visit frequencies and such
        pass

