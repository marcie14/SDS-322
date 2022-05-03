module newton
contains

! initiate function
real function f(x)
        real :: x
        f = x**3
end function

! intitate derivative
real function p(x)
        real :: x
        p = 3*(x**2)
end function

! initiate x_new
real function x_new(x)
        real :: x
        x_new = f(x) / p(x)
end function

! initiate convergence function
integer function convergence(x, eps, Max_Itns, i)
        real :: x, x1, xn, eps
        integer ::i, Max_Itns
        logical :: a, b, key

        ! create loop key
        a = .true.
        b = .true.
        key = a .and. b

        i = 1

        do while (key)
                xn = x_new(x) ! newtons method function
                x1 = x - xn

                i = i + 1 ! increment iterations

                ! update key with convergence criteria
                a = abs(x1 - x) > eps
                b = i < Max_Itns
                key = a .and. b

                ! update x for next loop
                x = x1
        end do

        ! return number of iterations
        convergence = i

end function

end module

program Main
        use newton
        implicit none

        real, parameter :: eps = 0.0001
        real :: n, x
        integer, parameter :: Max_Itns = 100
        integer :: i, ierror, iFinal
        real, dimension(:), allocatable :: xvals
        integer, dimension(:), allocatable :: ivals

        iFinal = 0

        do n = 0, 10, 0.01
                x = n
                i = 1

                iFinal = convergence(x, eps, Max_Itns, i)

                print *, x, n, i

        end do

end program Main
