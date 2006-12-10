! { dg-compile }
! { dg-shouldfail "Invalid Fortran 2003 code" }
! { dg-options "-std=f2003 -fall-intrinsics" }
! PR fortran/23994
!
! Test PROTECTED attribute. Within the module everything is allowed.
! Outside (use-associated): For pointers, their association status
! may not be changed. For nonpointers, their value may not be changed.
!
! Test of a invalid code

module protmod
  implicit none
  integer, Protected          :: a
  integer, protected, target  :: at
  integer, protected, pointer :: ap
end module protmod

program main
  use protmod
  implicit none
  a = 43       ! { dg-error "Assigning to PROTECTED variable" }
  ap => null() ! { dg-error "Assigning to PROTECTED variable" }
  nullify(ap)  ! { dg-error "Assigning to PROTECTED variable" }
  ap => at     ! { dg-error "Assigning to PROTECTED variable" }
  ap = 3       ! { dg-error "Assigning to PROTECTED variable" }
  allocate(ap) ! { dg-error "Assigning to PROTECTED variable" }
  ap = 73      ! { dg-error "Assigning to PROTECTED variable" }
  call increment(a,at) ! { dg-error "use-associated with PROTECTED attribute" }
contains
  subroutine increment(a1,a3)
    integer, intent(inout) :: a1, a3
    a1 = a1 + 1
    a3 = a3 + 1
  end subroutine increment
  subroutine pointer_assignments(p)
    integer, pointer :: p ! with [pointer] intent(out)
    p => null()           ! this is invalid
  end subroutine pointer_assignments
end program main

module prot2
  implicit none
contains
  subroutine bar
    real, protected :: b ! { dg-error "only allowed in specification part of a module" }
  end subroutine bar
end module prot2

! { dg-final { cleanup-modules "protmod" } }
