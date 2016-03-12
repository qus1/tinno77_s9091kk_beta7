#!/usr/bin/perl

use lib "mediatek/build/tools";
use pack_dep_gen;
PrintDependModule($0);

##########################################################
# Initialize Variables
##########################################################

# //<20120209-3044-Eric Lin, [secu]To separate modem.img for each projects.
my $cust_key = $ARGV[0];
my $prj = $ARGV[1];
my $modem_encode = $ARGV[2];
#my $modem_encode = "no";
my $modem_auth = $ARGV[3];
my $custom_dir = $ARGV[4];
my $secro_ac = $ARGV[5];
my $MTK_SECURITY_SW_SUPPORT = $ENV{"MTK_SECURITY_SW_SUPPORT"};
my $MTK_ROOT_CUSTOM_OUT = $ENV{"MTK_ROOT_CUSTOM_OUT"};
my $OUT_DIR = $ENV{OUT_DIR};
my $MTK_PLATFORM =$ENV{MTK_PLATFORM};
if (${MTK_PLATFORM} eq "MT6575" || ${MTK_PLATFORM} eq "MT6577")
{
    $legacy_mode = "1";
}
my $modem_cipher = "yes";

my $sml_dir = "mediatek/custom/$custom_dir/security/sml_auth";

my $cipher_tool = "mediatek/build/tools/CipherTool/CipherTool";
my $sign_tool = "mediatek/build/tools/SignTool/SignTool.sh";
my $secro_tool = "mediatek/build/tools/SecRo/SECRO_POST";
if (${legacy_mode} eq "1")
{
    $secro_tool = "mediatek/build/tools/SecRo/SECRO_POST_LEGACY";
}

my $cust_hspa= "_sec_hspa";
my $cust_modem= "$prj$cust_hspa";
# //>20120209-3044-Eric Lin
##########################################################
# Check Parameter
##########################################################
# //<20120209-3044-Eric Lin, [secu]To separate modem.img for each projects.
print "cust_key = $cust_key\n";
print "prj = $prj\n";
print "modem_encode = $modem_encode\n";
print "modem_auth = $modem_auth\n";
print "custom_dir = $custom_dir\n";
print "secro_ac = $secro_ac\n";

print "cust_modem    =  $cust_modem\n";
# //>20120209-3044-Eric Lin

print "\n\n";
print "********************************************\n";
print " CHECK PARAMETER \n";
print "********************************************\n";

if (${modem_auth} eq "yes")
{
	if (${modem_encode} eq "no")
	{
		die "Error! MTK_SEC_MODEM_AUTH is 'yes' but MTK_SEC_MODEM_ENCODE is 'no'\n";
	}
}

if (${MTK_SECURITY_SW_SUPPORT} ne "yes")
{
	$modem_cipher = "no"
}

print "parameter check pass (2 MDs)\n";
print "MTK_SEC_MODEM_AUTH    =  $modem_auth\n";
print "MTK_SEC_MODEM_ENCODE  =  $modem_encode\n";
print "MTK_SECURITY_SW_SUPPORT  =  $MTK_SECURITY_SW_SUPPORT\n";
print "MTK_ROOT_CUSTOM_OUT  =  $MTK_ROOT_CUSTOM_OUT\n";
print "modem_cipher  =  $modem_cipher\n";

##########################################################
# Process Modem Image
##########################################################

my $md_load = "$MTK_ROOT_CUSTOM_OUT/modem/modem.img";
my $b_md_load = "$MTK_ROOT_CUSTOM_OUT/modem/modem.img.bak";
my $c_md_load = "$MTK_ROOT_CUSTOM_OUT/modem/cipher_modem.img";
my $s_md_load = "$MTK_ROOT_CUSTOM_OUT/modem/signed_modem.img";

opendir(DIR, "$MTK_ROOT_CUSTOM_OUT/modem");
@files = grep(/\.img/,readdir(DIR));
foreach my $file (@files)
{
	$md_load = "$MTK_ROOT_CUSTOM_OUT/modem/$file";
	$b_md_load = "$MTK_ROOT_CUSTOM_OUT/modem/$file.bak";
	$c_md_load = "$MTK_ROOT_CUSTOM_OUT/modem/cipher_$file";
	$s_md_load = "$MTK_ROOT_CUSTOM_OUT/modem/signed_$file";
	&process_modem_image;
}
closedir(DIR);

sub process_modem_image
{
	print "\n\n";
	print "********************************************\n";
	print " PROCESS MODEM IMAGE ($md_load)\n";
	print "********************************************\n";	
#//<20120206-2499-Eric Lin, [secu] To backup the modem.img when you sign-image.
	my $md_mtk_load = "mediatek/custom/out/$prj/modem/modem.mtkkey.img";
	my $md_target_load = "mediatek/secutool/signedmd/$cust_modem/modem.img.signed.$cust_key";
#//>20120206-2499-Eric Lin
	
	if (-e "$b_md_load")
	{
		print "$md_load already processed ... \n";
	}
	else
	{
		if (-e "$md_load")
		{
			system("cp -f $md_load $b_md_load") == 0 or die "can't backup modem image";

			########################################		
			# Encrypt and Sign Modem Image
			########################################		
			if (${modem_encode} eq "yes")
			{
				if (${modem_cipher} eq "yes")
				{
					PrintDependency("$sml_dir/SML_ENCODE_KEY.ini");
					PrintDependency("$sml_dir/SML_ENCODE_CFG.ini");
					PrintDependency($md_load);
					PrintDependency($cipher_tool);
					system("./$cipher_tool ENC $sml_dir/SML_ENCODE_KEY.ini $sml_dir/SML_ENCODE_CFG.ini $md_load $c_md_load") == 0 or die "Cipher Tool return error\n";
				
					if(-e "$c_md_load")
					{
						system("rm -f $md_load") == 0 or die "can't remove original modem binary\n";
						system("mv -f $c_md_load $md_load") == 0 or die "can't generate cipher modem binary\n";
					}
				}
				PrintDependency("$sml_dir/SML_AUTH_KEY.ini");
				PrintDependency("$sml_dir/SML_AUTH_CFG.ini");
				PrintDependency("$md_load");
				PrintDependency($sign_tool);
				system("./$sign_tool $sml_dir/SML_AUTH_KEY.ini $sml_dir/SML_AUTH_CFG.ini $md_load $s_md_load");
	
				if(-e "$s_md_load")
				{
					system("rm -f $md_load") == 0 or die "can't remove original modem binary\n";
					system("mv -f $s_md_load $md_load") == 0 or die "can't generate signed modem binary\n";
				}			
#//<20120206-2499-Eric Lin, [secu] To backup the modem.img when you sign-image.
				system("cp -f  $md_load $md_mtk_load") ;
				system("cp -f  $md_load $md_target_load") ;
#//>20120206-2499-Eric Lin
			}
			else
			{
				print "doesn't execute Cipher Tool and Sign Tool ... \n";
			}
		}
		else
		{
			print "$md_load is not existed\n";			
		}
	}
}

##########################################################
# Fill AC_REGION
##########################################################

print "\n\n";
print "********************************************\n";
print " Fill AC_REGION \n";
print "********************************************\n";

my $secro_def_cfg = "mediatek/custom/common/secro/SECRO_DEFAULT_LOCK_CFG.ini";
if (${legacy_mode} eq "1")
{
    $secro_def_cfg = "mediatek/custom/common/secro/SECRO_DEFAULT_LOCK_CFG_LEGACY.ini";
}
# use $custom_dir to specify project only , not including flavor project part. ie:mt6582_evb , not mt6582_evb[tee]
my $secro_out_dir = "$OUT_DIR/target/product/$custom_dir/secro";
my $secro_out = "$secro_out_dir/AC_REGION";

my $secro_script = "mediatek/build/tools/SecRo/secro_post.pl";
PrintDependency($secro_def_cfg);

if (! -d "$secro_out_dir"){
    system("mkdir -p $secro_out_dir");
}

system("./$secro_script $secro_def_cfg $prj $custom_dir $secro_ac $secro_out") == 0 or die "SECRO post process return error\n";

##########################################################
# Process SECFL.ini
##########################################################

print "\n\n";
print "********************************************\n";
print " PROCESS SECFL.ini \n";
print "********************************************\n";

my $secfl_pl = "mediatek/build/tools/sign_sec_file_list.pl";
system("./$secfl_pl $custom_dir") == 0 or die "SECFL Perl return error\n";
#//<20120206-2926-Eric Lin, [secu] To separate the build sign flows by script file.
my $moto_signed_modem = "mediatek/secutool/signedmd/$cust_modem/modem.img.signed.$cust_key";
$md_load = "mediatek/custom/out/$prj/modem/modem.img";
if (${cust_key} eq "KEY_TYPE_MP" || ${cust_key} eq "KEY_TYPE_DEV")
{
	print "[arima][mp key] Non bypass that  copy signed modem back to out folder.\n";
	if (-e "$moto_signed_modem")
	{
		system("cp -f $moto_signed_modem $md_load") ;
		print "[arima][mp key] copy done.";
	}
	else
	{
		die "doesn't exist: $moto_signed_modem\n";
	}		 
}
else
{
	print "[arima][mtk key] To bypass that  copy signed modem back to out folder.\n";
}
# //>20120206-2926-Eric Lin
