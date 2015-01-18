from django.db import models

from django.contrib.auth.models import User

# Create your models here.

class UserProfile(models.Model):
	user = models.OneToOneField(User)
	context_io_account_id = models.TextField(null=True)
